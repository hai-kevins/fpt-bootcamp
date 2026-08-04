#include "clock.h"
#include "cortex_m3_port.h"
#include "gpio.h"
#include "panic.h"
#include "scheduler.h"
#include "task.h"
#include "uart.h"

#include <stdint.h>

static rtos_task_t g_first_task;
static rtos_task_t g_idle_task;

static uint32_t g_first_stack[128] __attribute__((aligned(8)));
static uint32_t g_idle_stack[96] __attribute__((aligned(8)));

static uint32_t g_magic = 0xC0FFEE01UL;

static void first_task(void *argument)
{
    const uint32_t received = *(const uint32_t *) argument;

    uart1_write_string("\r\nLab 03 first task on PSP\r\nargument=");
    uart1_write_hex32(received);
    uart1_write_string(" psp=");
    uart1_write_hex32(rtos_port_get_psp());
    uart1_write_string(" msp=");
    uart1_write_hex32(rtos_port_get_msp());
    uart1_write_string("\r\n");

    for (;;)
    {
        gpio_led_toggle();
        for (volatile uint32_t delay = 0U; delay < 80000U; ++delay)
        {
            __asm volatile("nop");
        }
    }
}

static void idle_task(void *argument)
{
    (void) argument;
    for (;;)
    {
        rtos_port_wait_for_interrupt();
    }
}

int main(void)
{
    clock_init_hsi_8mhz();
    gpio_led_init();
    uart1_init_9600_hsi8();
    rtos_scheduler_init();

    RTOS_ASSERT(rtos_task_create_static(&g_first_task, "first", 0U, 0U, first_task, &g_magic, g_first_stack, 128U));
    RTOS_ASSERT(rtos_task_create_static(&g_idle_task, "idle", 1U, 3U, idle_task, (void *)0, g_idle_stack, 96U));
    RTOS_ASSERT(rtos_scheduler_add_task(&g_first_task));
    RTOS_ASSERT(rtos_scheduler_add_task(&g_idle_task));
    rtos_scheduler_start();
}
