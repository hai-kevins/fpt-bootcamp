#include "benchmark_clock.h"
#include "benchmark_console.h"
#include "benchmark_stats.h"
#include "clock.h"
#include "cortex_m3_port.h"
#include "gpio.h"
#include "message_pool.h"
#include "panic.h"
#include "queue.h"
#include "scheduler.h"
#include "semaphore.h"
#include "software_timer.h"
#include "systick.h"
#include "task.h"
#include "timer_service.h"
#include "uart.h"
#include <stdint.h>

#define STACK_SMALL   (144U)
#define STACK_MEDIUM  (192U)
#define STACK_MONITOR (320U)
#define STACK_IDLE    (96U)
#define MESSAGE_QUEUE_CAPACITY (8U)
#define MESSAGE_POOL_BLOCKS (8U)
#define MESSAGE_POOL_BLOCK_SIZE (32U)

typedef struct
{
    uint32_t sequence;
    uint32_t timestamp;
    uint32_t value;
    uint32_t source;
} app_message_t;

static hr_task_t g_producer_task;
static hr_task_t g_consumer_task;
static hr_task_t g_timer_service_task;
static hr_task_t g_switch_a_task;
static hr_task_t g_switch_b_task;
static hr_task_t g_monitor_task;
static hr_task_t g_idle_task;

static uint32_t g_producer_stack[STACK_SMALL] __attribute__((aligned(8)));
static uint32_t g_consumer_stack[STACK_MEDIUM] __attribute__((aligned(8)));
static uint32_t g_timer_service_stack[STACK_MEDIUM] __attribute__((aligned(8)));
static uint32_t g_switch_a_stack[STACK_SMALL] __attribute__((aligned(8)));
static uint32_t g_switch_b_stack[STACK_SMALL] __attribute__((aligned(8)));
static uint32_t g_monitor_stack[STACK_MONITOR] __attribute__((aligned(8)));
static uint32_t g_idle_stack[STACK_IDLE] __attribute__((aligned(8)));

static hr_queue_t g_message_queue;
static app_message_t g_queue_storage[MESSAGE_QUEUE_CAPACITY];
static hr_semaphore_t g_timer_signal;

static hr_message_pool_t g_message_pool;

static uint8_t g_pool_memory[MESSAGE_POOL_BLOCKS * MESSAGE_POOL_BLOCK_SIZE] __attribute__((aligned(sizeof(void *))));

static uint8_t g_pool_map[MESSAGE_POOL_BLOCKS];

static hr_software_timer_t g_one_shot_timer;
static hr_software_timer_t g_periodic_timer;

static hr_benchmark_stats_t g_switch_stats;
static hr_benchmark_stats_t g_message_stats;
static hr_benchmark_stats_t g_timer_stats;
static const uint32_t g_bins[HR_BENCHMARK_BIN_COUNT] =
{
    16U,
    32U,
    64U,
    128U,
    256U,
    512U,
    1024U,
    4096U
};

static volatile uint32_t g_switch_turn;
static volatile uint32_t g_switch_start;
static volatile uint32_t g_periodic_toggle_count;
static volatile uint32_t g_one_shot_count;
static volatile uint32_t g_timer_expected;

static void periodic_callback(void *argument)
{
    (void) argument;
    ++g_periodic_toggle_count;
    gpio_led_toggle();
}

static void one_shot_callback(void *argument)
{
    const uint32_t now = hr_benchmark_now();
    (void) argument;
    ++g_one_shot_count;
    hr_benchmark_stats_add(&g_timer_stats, now - g_timer_expected);
}

static void producer_task(void *argument)
{
    uint32_t sequence = 0U;
    uint32_t next = hr_scheduler_tick_now();
    (void) argument;
    for (;;)
    {
        app_message_t message;
        message.sequence = sequence++;
        message.value = sequence ^ 0x55AA55AAUL;
        message.source = 1U;
        message.timestamp = hr_benchmark_now();
        (void) hr_queue_send(&g_message_queue, &message, 50U);
        if ((sequence & 0x0FUL) == 0U)
        {
            void *block = hr_message_pool_alloc(&g_message_pool);
            if (block != (void *)0)
            {
                (void) hr_message_pool_free(&g_message_pool, block);
            }
        }
        (void) hr_task_delay_until(&next, 25U);
    }
}

static void consumer_task(void *argument)
{
    app_message_t message;
    (void) argument;
    for (;;)
    {
        if (hr_queue_receive(&g_message_queue, &message, HR_WAIT_FOREVER) == HR_WAIT_SUCCESS)
        {
            hr_benchmark_stats_add(&g_message_stats, hr_benchmark_elapsed(message.timestamp, hr_benchmark_now()));
        }
    }
}

static void switch_a_task(void *argument)
{
    (void) argument;
    for (;;)
    {
        if (g_switch_turn == 0U)
        {
            g_switch_start = hr_benchmark_now();
            g_switch_turn = 1U;
            hr_task_yield();
        }
        else
        {
            (void) hr_task_delay(1U);
        }
    }
}

static void switch_b_task(void *argument)
{
    (void) argument;
    for (;;)
    {
        if (g_switch_turn == 1U)
        {
            hr_benchmark_stats_add(&g_switch_stats, hr_benchmark_elapsed(g_switch_start, hr_benchmark_now()));
            g_switch_turn = 0U;
            hr_task_yield();
        }
        else
        {
            (void) hr_task_delay(1U);
        }
    }
}

static void monitor_task(void *argument)
{
    (void) argument;
    uart1_write_string("\r\nHairRTOS Communication and Benchmark Console\r\n");
    uart1_write_string("STM32F103 | HSI 8 MHz | SysTick 1 kHz\r\n");
    hr_benchmark_console_print_help();
    for (;;)
    {
        if (uart1_byte_available())
        {
            hr_benchmark_console_process(uart1_read_byte());
        }
        if ((g_one_shot_timer.state == HR_TIMER_INACTIVE) && ((hr_scheduler_tick_now() % 2000U) < 25U))
        {
            g_timer_expected = hr_benchmark_now() + (8000U * 100U);
            (void) hr_software_timer_start(&g_one_shot_timer, 100U, 0U);
        }
        (void) hr_task_delay(25U);
    }
}

static void idle_task(void *argument)
{
    (void) argument;
    for (;;)
    {
        hr_port_wait_for_interrupt();
    }
}

static void create_tasks(void)
{
    HR_ASSERT(hr_task_create_static(&g_timer_service_task, "timer-service", 0U, 0U, hr_timer_service_task, (void *)0,
        g_timer_service_stack, STACK_MEDIUM));
    HR_ASSERT(hr_task_create_static(&g_consumer_task, "consumer", 1U, 1U, consumer_task, (void *)0, g_consumer_stack,
        STACK_MEDIUM));
    HR_ASSERT(hr_task_create_static(&g_switch_a_task, "switch-a", 2U, 1U, switch_a_task, (void *)0, g_switch_a_stack,
        STACK_SMALL));
    HR_ASSERT(hr_task_create_static(&g_switch_b_task, "switch-b", 3U, 1U, switch_b_task, (void *)0, g_switch_b_stack,
        STACK_SMALL));
    HR_ASSERT(hr_task_create_static(&g_producer_task, "producer", 4U, 2U, producer_task, (void *)0, g_producer_stack,
        STACK_SMALL));
    HR_ASSERT(hr_task_create_static(&g_monitor_task, "monitor", 5U, 2U, monitor_task, (void *)0, g_monitor_stack,
        STACK_MONITOR));
    HR_ASSERT(hr_task_create_static(&g_idle_task, "idle", 6U, 3U, idle_task, (void *)0, g_idle_stack, STACK_IDLE));

    HR_ASSERT(hr_scheduler_add_task(&g_timer_service_task));
    HR_ASSERT(hr_scheduler_add_task(&g_consumer_task));
    HR_ASSERT(hr_scheduler_add_task(&g_switch_a_task));
    HR_ASSERT(hr_scheduler_add_task(&g_switch_b_task));
    HR_ASSERT(hr_scheduler_add_task(&g_producer_task));
    HR_ASSERT(hr_scheduler_add_task(&g_monitor_task));
    HR_ASSERT(hr_scheduler_add_task(&g_idle_task));
}

int main(void)
{
    clock_init_hsi_8mhz();
    gpio_led_init();
    uart1_init_9600_hsi8();
    hr_benchmark_clock_init();
    hr_scheduler_init();

    HR_ASSERT(hr_queue_init_static(&g_message_queue, "messages", g_queue_storage, MESSAGE_QUEUE_CAPACITY, sizeof(app_message_t)));
    HR_ASSERT(hr_message_pool_init_static(&g_message_pool, "message-pool", g_pool_memory, MESSAGE_POOL_BLOCK_SIZE,
        MESSAGE_POOL_BLOCKS, g_pool_map));
    HR_ASSERT(hr_semaphore_init(&g_timer_signal, "timer-signal", 0U, 1U));
    hr_software_timer_system_init(&g_timer_signal);
    hr_timer_service_bind(&g_timer_signal);
    HR_ASSERT(hr_software_timer_init(&g_one_shot_timer, "one-shot", one_shot_callback, (void *)0));
    HR_ASSERT(hr_software_timer_init(&g_periodic_timer, "periodic", periodic_callback, (void *)0));

    hr_benchmark_stats_init(&g_switch_stats, g_bins, HR_BENCHMARK_BIN_COUNT);
    hr_benchmark_stats_init(&g_message_stats, g_bins, HR_BENCHMARK_BIN_COUNT);
    hr_benchmark_stats_init(&g_timer_stats, g_bins, HR_BENCHMARK_BIN_COUNT);
    hr_benchmark_console_bind(&g_message_queue, &g_message_pool, &g_one_shot_timer, &g_periodic_timer, &g_switch_stats,
        &g_message_stats, &g_timer_stats);

    create_tasks();
    HR_ASSERT(hr_software_timer_start(&g_periodic_timer, 500U, 500U));
    systick_init_1khz();
    hr_scheduler_start();
}
