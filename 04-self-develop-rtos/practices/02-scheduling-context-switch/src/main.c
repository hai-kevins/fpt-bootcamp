#include "clock.h"
#include "cortex_m3_port.h"
#include "gpio.h"
#include "panic.h"
#include "scheduler.h"
#include "scheduler_trace.h"
#include "systick.h"
#include "task.h"
#include "uart.h"

#include <stddef.h>
#include <stdint.h>

#define STACK_WORDS_SMALL   (160U)
#define STACK_WORDS_MONITOR (256U)

static rtos_task_t g_high_task;
static rtos_task_t g_worker_a_task;
static rtos_task_t g_worker_b_task;
static rtos_task_t g_monitor_task;
static rtos_task_t g_idle_task;

static uint32_t g_high_stack[STACK_WORDS_SMALL] __attribute__((aligned(8)));
static uint32_t g_worker_a_stack[STACK_WORDS_SMALL] __attribute__((aligned(8)));
static uint32_t g_worker_b_stack[STACK_WORDS_SMALL] __attribute__((aligned(8)));
static uint32_t g_monitor_stack[STACK_WORDS_MONITOR] __attribute__((aligned(8)));
static uint32_t g_idle_stack[STACK_WORDS_SMALL] __attribute__((aligned(8)));

volatile uint32_t g_high_runs;
volatile uint32_t g_worker_a_counter;
volatile uint32_t g_worker_b_counter;
volatile uint32_t g_idle_counter;

static void print_newline(void)
{
    uart1_write_string("\r\n");
}

static void print_task(const rtos_task_t *task)
{
    uart1_write_string("id=");
    uart1_write_u32(task->id);
    uart1_write_string(" name=");
    uart1_write_string(task->name);
    uart1_write_string(" prio=");
    uart1_write_u32(task->priority);
    uart1_write_string(" state=");
    uart1_write_u32((uint32_t) task->state);
    uart1_write_string(" switches=");
    uart1_write_u32(task->switch_count);
    uart1_write_string(" runtime_ticks=");
    uart1_write_u32(task->runtime_ticks);
    uart1_write_string(" unused_stack_words=");
    uart1_write_u32((uint32_t) rtos_task_stack_unused_words(task));
    print_newline();
}

static void print_stats(void)
{
    uart1_write_string("tick=");
    uart1_write_u32(g_kernel_tick);
    uart1_write_string(" switches=");
    uart1_write_u32(g_context_switch_count);
    uart1_write_string(" ready_bitmap=");
    uart1_write_hex32(rtos_scheduler_ready_bitmap());
    print_newline();

    uart1_write_string("high=");
    uart1_write_u32(g_high_runs);
    uart1_write_string(" worker_a=");
    uart1_write_u32(g_worker_a_counter);
    uart1_write_string(" worker_b=");
    uart1_write_u32(g_worker_b_counter);
    uart1_write_string(" idle=");
    uart1_write_u32(g_idle_counter);
    print_newline();

    for (size_t index = 0U; index < rtos_scheduler_task_count(); ++index)
    {
        const rtos_task_t *task = rtos_scheduler_task_at(index);
        if (task != (const rtos_task_t *)0)
        {
            print_task(task);
        }
    }
}

static void print_trace(void)
{
    rtos_trace_record_t records[16];
    const size_t count = rtos_trace_snapshot(records, 16U);

    for (size_t index = 0U; index < count; ++index)
    {
        uart1_write_string("seq=");
        uart1_write_u32(records[index].sequence);
        uart1_write_string(" tick=");
        uart1_write_u32(records[index].tick);
        uart1_write_string(" type=");
        uart1_write_u32((uint32_t) records[index].type);
        uart1_write_string(" from=");
        uart1_write_u32(records[index].from_task);
        uart1_write_string(" to=");
        uart1_write_u32(records[index].to_task);
        print_newline();
    }
}

static void high_event_task(void *argument)
{
    (void) argument;

    for (;;)
    {
        ++g_high_runs;
        gpio_led_toggle();
        rtos_task_delay(500U);
    }
}

static void worker_a_task(void *argument)
{
    uint32_t local = 0xA0000000UL;
    (void) argument;

    for (;;)
    {
        ++local;
        g_worker_a_counter = local;

        if ((local & 0x3FFFUL) == 0U)
        {
            rtos_task_delay(50U);
        }
    }
}

static void worker_b_task(void *argument)
{
    uint32_t local = 0xB0000000UL;
    (void) argument;

    for (;;)
    {
        local += 3U;
        g_worker_b_counter = local;

        if ((local & 0x3FFFUL) == 0U)
        {
            rtos_task_delay(50U);
        }
    }
}

static void monitor_task(void *argument)
{
    (void) argument;

    uart1_write_string("\r\nRTOS Scheduler Playground\r\n""h=help s=stats t=trace\r\n");

    for (;;)
    {
        if (uart1_byte_available())
        {
            const uint8_t command = uart1_read_byte();

            if (command == (uint8_t)'h')
            {
                uart1_write_string("h help | s stats | t trace\r\n");
            }
            else if (command == (uint8_t)'s')
            {
                print_stats();
            }
            else if (command == (uint8_t)'t')
            {
                print_trace();
            }
            else
            {
                uart1_write_string("unknown command\r\n");
            }
        }

        rtos_task_delay(20U);
    }
}

static void idle_task(void *argument)
{
    (void) argument;

    for (;;)
    {
        ++g_idle_counter;
        rtos_port_wait_for_interrupt();
    }
}

static void create_tasks(void)
{
    RTOS_ASSERT(rtos_task_create_static(&g_high_task, "high", 0U, 0U, high_event_task, (void *)0, g_high_stack,
        STACK_WORDS_SMALL));
    RTOS_ASSERT(rtos_task_create_static(&g_worker_a_task, "worker-a", 1U, 1U, worker_a_task, (void *)0, g_worker_a_stack,
        STACK_WORDS_SMALL));
    RTOS_ASSERT(rtos_task_create_static(&g_worker_b_task, "worker-b", 2U, 1U, worker_b_task, (void *)0, g_worker_b_stack,
        STACK_WORDS_SMALL));
    RTOS_ASSERT(rtos_task_create_static(&g_monitor_task, "monitor", 3U, 2U, monitor_task, (void *)0, g_monitor_stack,
        STACK_WORDS_MONITOR));
    RTOS_ASSERT(rtos_task_create_static(&g_idle_task, "idle", 4U, 3U, idle_task, (void *)0, g_idle_stack, STACK_WORDS_SMALL));

    RTOS_ASSERT(rtos_scheduler_add_task(&g_high_task));
    RTOS_ASSERT(rtos_scheduler_add_task(&g_worker_a_task));
    RTOS_ASSERT(rtos_scheduler_add_task(&g_worker_b_task));
    RTOS_ASSERT(rtos_scheduler_add_task(&g_monitor_task));
    RTOS_ASSERT(rtos_scheduler_add_task(&g_idle_task));
}

int main(void)
{
    clock_init_hsi_8mhz();
    gpio_led_init();
    uart1_init_9600_hsi8();

    rtos_scheduler_init();
    create_tasks();
    systick_init_1khz();
    rtos_scheduler_start();
}
