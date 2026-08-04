#include "clock.h"
#include "cortex_m3_port.h"
#include "gpio.h"
#include "panic.h"
#include "scheduler.h"
#include "stm32f1.h"
#include "systick.h"
#include "task.h"
#include "uart.h"

#include <stdint.h>

static rtos_task_t high_t, low_t, idle_t;

static uint32_t sh[128] __attribute__((aligned(8)));
static uint32_t sl[128] __attribute__((aligned(8)));
static uint32_t si[96] __attribute__((aligned(8)));

volatile uint32_t high_runs, low_runs, irq_count;

void EXTI0_IRQHandler(void)
{
    if ((EXTI_PR & 1UL) != 0U)
    {
        EXTI_PR = 1UL;
        ++irq_count;
        (void) rtos_task_wake_from_isr(&high_t);
    }
}

static void high(void *arg)
{
    (void) arg;
    for (;;)
    {
        ++high_runs;
        gpio_led_toggle();
        rtos_task_block_current();
    }
}

static void low(void *arg)
{
    (void) arg;
    for (;;)
    {
        ++low_runs;
    }
}

static void idle(void *arg)
{
    (void) arg;
    for (;;)
    {
        rtos_port_wait_for_interrupt();
    }
}

int main(void)
{
    clock_init_hsi_8mhz();
    gpio_led_init();
    gpio_button_exti0_init();
    uart1_init_9600_hsi8();
    uart1_write_string("\r\nLab 08 EXTI preemption\r\n");
    rtos_scheduler_init();
    RTOS_ASSERT(rtos_task_create_static(&high_t, "high", 0U, 0U, high, 0, sh, 128U));
    RTOS_ASSERT(rtos_task_create_static(&low_t, "low", 1U, 2U, low, 0, sl, 128U));
    RTOS_ASSERT(rtos_task_create_static(&idle_t, "idle", 2U, 3U, idle, 0, si, 96U));
    RTOS_ASSERT(rtos_scheduler_add_task(&high_t));
    RTOS_ASSERT(rtos_scheduler_add_task(&low_t));
    RTOS_ASSERT(rtos_scheduler_add_task(&idle_t));
    systick_init_1khz();
    rtos_scheduler_start();
}
