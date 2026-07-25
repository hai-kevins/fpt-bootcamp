#include "clock.h"
#include "cortex_m3_port.h"
#include "gpio.h"
#include "mutex.h"
#include "panic.h"
#include "scheduler.h"
#include "semaphore.h"
#include "stm32f1.h"
#include "synchronization_inspector.h"
#include "systick.h"
#include "task.h"
#include "uart.h"
#include <stdbool.h>
#include <stdint.h>

#define STACK_SMALL   (128U)
#define STACK_MEDIUM  (192U)
#define STACK_MONITOR (320U)
#define STACK_IDLE    (96U)

static rtos_task_t g_event_task;
static rtos_task_t g_high_task;
static rtos_task_t g_medium_task;
static rtos_task_t g_low_task;
static rtos_task_t g_producer_task;
static rtos_task_t g_consumer_task;
static rtos_task_t g_monitor_task;
static rtos_task_t g_idle_task;

static uint32_t g_event_stack[STACK_SMALL] __attribute__((aligned(8)));
static uint32_t g_high_stack[STACK_MEDIUM] __attribute__((aligned(8)));
static uint32_t g_medium_stack[STACK_SMALL] __attribute__((aligned(8)));
static uint32_t g_low_stack[STACK_MEDIUM] __attribute__((aligned(8)));
static uint32_t g_producer_stack[STACK_SMALL] __attribute__((aligned(8)));
static uint32_t g_consumer_stack[STACK_SMALL] __attribute__((aligned(8)));
static uint32_t g_monitor_stack[STACK_MONITOR] __attribute__((aligned(8)));
static uint32_t g_idle_stack[STACK_IDLE] __attribute__((aligned(8)));

static rtos_semaphore_t g_button_event;
static rtos_semaphore_t g_items;
static rtos_mutex_t g_resource_mutex;

volatile uint32_t g_event_count;
volatile uint32_t g_high_count;
volatile uint32_t g_medium_count;
volatile uint32_t g_low_count;
volatile uint32_t g_produced_count;
volatile uint32_t g_consumed_count;
volatile uint32_t g_idle_count;

static void event_task(void *argument)
{
    (void)argument;
    for (;;)
    {
        if (rtos_semaphore_take(&g_button_event, RTOS_WAIT_FOREVER) == RTOS_WAIT_SUCCESS)
        {
            ++g_event_count;
            gpio_led_toggle();
        }
    }
}

static void high_task(void *argument)
{
    (void)argument;
    (void)rtos_task_delay(20U);
    for (;;)
    {
        if (rtos_mutex_lock(&g_resource_mutex, 100U) == RTOS_WAIT_SUCCESS)
        {
            ++g_high_count;
            (void)rtos_mutex_unlock(&g_resource_mutex);
        }
        (void)rtos_task_delay(100U);
    }
}

static void medium_task(void *argument)
{
    uint32_t local = 0U;
    (void)argument;
    for (;;)
    {
        ++local;
        g_medium_count = local;
        if ((local & 0x3FFFUL) == 0U) { rtos_task_yield(); }
    }
}

static void low_task(void *argument)
{
    uint32_t cycle = 0U;
    (void)argument;
    for (;;)
    {
        if (rtos_mutex_lock(&g_resource_mutex, RTOS_WAIT_FOREVER) == RTOS_WAIT_SUCCESS)
        {
            for (uint32_t i = 0U; i < 50000U; ++i)
            {
                ++g_low_count;
                if ((i & 0x0FFFUL) == 0U) { rtos_task_yield(); }
            }
            (void)rtos_mutex_unlock(&g_resource_mutex);
        }
        ++cycle;
        (void)rtos_task_delay(250U + (cycle & 0x1FUL));
    }
}

static void producer_task(void *argument)
{
    (void)argument;
    for (;;)
    {
        if (rtos_semaphore_give(&g_items)) { ++g_produced_count; }
        (void)rtos_task_delay(40U);
    }
}

static void consumer_task(void *argument)
{
    (void)argument;
    for (;;)
    {
        if (rtos_semaphore_take(&g_items, 200U) == RTOS_WAIT_SUCCESS)
        {
            ++g_consumed_count;
        }
    }
}

static void monitor_task(void *argument)
{
    (void)argument;
    uart1_write_string("\r\nRTOS Synchronization Playground\r\n");
    rtos_sync_inspector_print_help();
    for (;;)
    {
        if (uart1_byte_available())
        {
            const uint8_t command = uart1_read_byte();
            if (command == (uint8_t)'h') { rtos_sync_inspector_print_help(); }
            else if (command == (uint8_t)'t') { rtos_sync_inspector_print_tasks(); }
            else if (command == (uint8_t)'d') { rtos_sync_inspector_print_delayed(); }
            else if (command == (uint8_t)'s') { rtos_sync_inspector_print_semaphores(); }
            else if (command == (uint8_t)'m') { rtos_sync_inspector_print_mutex(); }
            else if (command == (uint8_t)'v') { rtos_sync_inspector_print_validation(); }
            else if (command == (uint8_t)'u')
            {
                if (g_medium_task.state == RTOS_TASK_SUSPENDED)
                { (void)rtos_task_resume(&g_medium_task); uart1_write_string("medium resumed\r\n"); }
                else
                { (void)rtos_task_suspend(&g_medium_task); uart1_write_string("medium suspended\r\n"); }
            }
            else { uart1_write_string("unknown command\r\n"); }
        }
        (void)rtos_task_delay(25U);
    }
}

static void idle_task(void *argument)
{
    (void)argument;
    for (;;)
    {
        ++g_idle_count;
        rtos_port_wait_for_interrupt();
    }
}

static void create_tasks(void)
{
    RTOS_ASSERT(rtos_task_create_static(&g_event_task, "event", 0U, 0U, event_task,
                                    (void *)0, g_event_stack, STACK_SMALL));
    RTOS_ASSERT(rtos_task_create_static(&g_high_task, "high", 1U, 0U, high_task,
                                    (void *)0, g_high_stack, STACK_MEDIUM));
    RTOS_ASSERT(rtos_task_create_static(&g_medium_task, "medium", 2U, 1U, medium_task,
                                    (void *)0, g_medium_stack, STACK_SMALL));
    RTOS_ASSERT(rtos_task_create_static(&g_low_task, "low", 3U, 2U, low_task,
                                    (void *)0, g_low_stack, STACK_MEDIUM));
    RTOS_ASSERT(rtos_task_create_static(&g_producer_task, "producer", 4U, 2U, producer_task,
                                    (void *)0, g_producer_stack, STACK_SMALL));
    RTOS_ASSERT(rtos_task_create_static(&g_consumer_task, "consumer", 5U, 1U, consumer_task,
                                    (void *)0, g_consumer_stack, STACK_SMALL));
    RTOS_ASSERT(rtos_task_create_static(&g_monitor_task, "monitor", 6U, 2U, monitor_task,
                                    (void *)0, g_monitor_stack, STACK_MONITOR));
    RTOS_ASSERT(rtos_task_create_static(&g_idle_task, "idle", 7U, 3U, idle_task,
                                    (void *)0, g_idle_stack, STACK_IDLE));

    RTOS_ASSERT(rtos_scheduler_add_task(&g_event_task));
    RTOS_ASSERT(rtos_scheduler_add_task(&g_high_task));
    RTOS_ASSERT(rtos_scheduler_add_task(&g_medium_task));
    RTOS_ASSERT(rtos_scheduler_add_task(&g_low_task));
    RTOS_ASSERT(rtos_scheduler_add_task(&g_producer_task));
    RTOS_ASSERT(rtos_scheduler_add_task(&g_consumer_task));
    RTOS_ASSERT(rtos_scheduler_add_task(&g_monitor_task));
    RTOS_ASSERT(rtos_scheduler_add_task(&g_idle_task));
}

void EXTI0_IRQHandler(void)
{
    bool higher = false;
    EXTI_PR = 1UL;
    (void)rtos_semaphore_give_from_isr(&g_button_event, &higher);
    (void)higher; /* give_from_isr already pends PendSV through wake path. */
}

int main(void)
{
    clock_init_hsi_8mhz();
    gpio_led_init();
    gpio_button_exti0_init();
    uart1_init_9600_hsi8();
    rtos_scheduler_init();
    RTOS_ASSERT(rtos_semaphore_init(&g_button_event, "button-event", 0U, 1U));
    RTOS_ASSERT(rtos_semaphore_init(&g_items, "items", 0U, 8U));
    RTOS_ASSERT(rtos_mutex_init(&g_resource_mutex, "resource"));
    rtos_sync_inspector_bind(&g_button_event, &g_items, &g_resource_mutex);
    create_tasks();
    systick_init_1khz();
    rtos_scheduler_start();
}
