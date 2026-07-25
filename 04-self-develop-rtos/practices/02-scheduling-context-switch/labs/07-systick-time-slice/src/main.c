#include "clock.h"
#include "cortex_m3_port.h"
#include "gpio.h"
#include "panic.h"
#include "scheduler.h"
#include "systick.h"
#include "task.h"
#include "uart.h"

#include <stdint.h>

static rtos_task_t a, b, idle_t;
static uint32_t sa[128] __attribute__((aligned(8)));
static uint32_t sb[128] __attribute__((aligned(8)));
static uint32_t si[96] __attribute__((aligned(8)));
volatile uint32_t worker_a_count, worker_b_count;

static void worker_a(void *arg)
{
    uint32_t local = 0U; (void)arg;
    for (;;) { ++local; worker_a_count = local; }
}
static void worker_b(void *arg)
{
    uint32_t local = 0U; (void)arg;
    for (;;) { local += 2U; worker_b_count = local; }
}
static void idle(void *arg)
{
    (void)arg; for (;;) { rtos_port_wait_for_interrupt(); }
}

int main(void)
{
    clock_init_hsi_8mhz(); gpio_led_init(); uart1_init_9600_hsi8();
    uart1_write_string("\r\nLab 07 SysTick time slice\r\n");
    rtos_scheduler_init();
    RTOS_ASSERT(rtos_task_create_static(&a,"A",0U,1U,worker_a,0,sa,128U));
    RTOS_ASSERT(rtos_task_create_static(&b,"B",1U,1U,worker_b,0,sb,128U));
    RTOS_ASSERT(rtos_task_create_static(&idle_t,"idle",2U,3U,idle,0,si,96U));
    RTOS_ASSERT(rtos_scheduler_add_task(&a)); RTOS_ASSERT(rtos_scheduler_add_task(&b));
    RTOS_ASSERT(rtos_scheduler_add_task(&idle_t));
    systick_init_1khz(); rtos_scheduler_start();
}
