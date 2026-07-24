#include "clock.h"
#include "cortex_m3_port.h"
#include "gpio.h"
#include "panic.h"
#include "scheduler.h"
#include "systick.h"
#include "task.h"
#include "task_inspector.h"
#include "uart.h"
#include <stdint.h>

#define STACK_WORDS_WORKER  (160U)
#define STACK_WORDS_MONITOR (320U)
#define STACK_WORDS_IDLE    (96U)

static hr_task_t g_worker_a_task;
static hr_task_t g_worker_b_task;
static hr_task_t g_monitor_task;
static hr_task_t g_idle_task;

static uint32_t g_worker_a_stack[STACK_WORDS_WORKER] __attribute__((aligned(8)));
static uint32_t g_worker_b_stack[STACK_WORDS_WORKER] __attribute__((aligned(8)));
static uint32_t g_monitor_stack[STACK_WORDS_MONITOR] __attribute__((aligned(8)));
static uint32_t g_idle_stack[STACK_WORDS_IDLE] __attribute__((aligned(8)));

volatile uint32_t g_worker_a_counter;
volatile uint32_t g_worker_b_counter;
volatile uint32_t g_idle_counter;

static void worker_a(void *argument)
{
    uint32_t local = 0xA0000000UL;
    (void)argument;
    for (;;)
    {
        ++local;
        g_worker_a_counter = local;
        if ((local & 0x1FFFUL) == 0U) { hr_task_delay(15U); }
    }
}

static void worker_b(void *argument)
{
    uint32_t local = 0xB0000000UL;
    (void)argument;
    for (;;)
    {
        local += 3U;
        g_worker_b_counter = local;
        if ((local & 0x1FFFUL) == 0U) { hr_task_delay(25U); }
    }
}

static void monitor(void *argument)
{
    (void)argument;
    uart1_write_string("\r\nHairRTOS Task Inspector\r\n");
    hr_task_inspector_print_help();
    for (;;)
    {
        if (uart1_byte_available())
        {
            const uint8_t command = uart1_read_byte();
            if (command == (uint8_t)'h') { hr_task_inspector_print_help(); }
            else if (command == (uint8_t)'t') { hr_task_inspector_print_tasks(); }
            else if (command == (uint8_t)'r') { hr_task_inspector_print_ready(); }
            else if (command == (uint8_t)'c') { hr_task_inspector_print_current(); }
            else if (command == (uint8_t)'s') { hr_task_inspector_print_stacks(); }
            else if (command == (uint8_t)'v') { hr_task_inspector_print_validation(); }
            else { uart1_write_string("unknown command\r\n"); }
        }
        gpio_led_toggle();
        hr_task_delay(100U);
    }
}

static void idle(void *argument)
{
    (void)argument;
    for (;;) { ++g_idle_counter; hr_port_wait_for_interrupt(); }
}

static void create_tasks(void)
{
    HR_ASSERT(hr_task_create_static(&g_worker_a_task, "worker-a", 0U, 1U, worker_a,
                                    (void *)0, g_worker_a_stack, STACK_WORDS_WORKER));
    HR_ASSERT(hr_task_create_static(&g_worker_b_task, "worker-b", 1U, 1U, worker_b,
                                    (void *)0, g_worker_b_stack, STACK_WORDS_WORKER));
    HR_ASSERT(hr_task_create_static(&g_monitor_task, "monitor", 2U, 2U, monitor,
                                    (void *)0, g_monitor_stack, STACK_WORDS_MONITOR));
    HR_ASSERT(hr_task_create_static(&g_idle_task, "idle", 3U, 3U, idle,
                                    (void *)0, g_idle_stack, STACK_WORDS_IDLE));
    HR_ASSERT(hr_scheduler_add_task(&g_worker_a_task));
    HR_ASSERT(hr_scheduler_add_task(&g_worker_b_task));
    HR_ASSERT(hr_scheduler_add_task(&g_monitor_task));
    HR_ASSERT(hr_scheduler_add_task(&g_idle_task));
}

int main(void)
{
    clock_init_hsi_8mhz();
    gpio_led_init();
    uart1_init_9600_hsi8();
    hr_scheduler_init();
    create_tasks();
    systick_init_1khz();
    hr_scheduler_start();
}
