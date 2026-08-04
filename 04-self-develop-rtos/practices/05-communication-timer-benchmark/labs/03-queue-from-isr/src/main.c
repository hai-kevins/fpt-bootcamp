#include "clock.h"
#include "cortex_m3_port.h"
#include "gpio.h"
#include "panic.h"
#include "queue.h"
#include "scheduler.h"
#include "stm32f1.h"
#include "systick.h"
#include "task.h"
#include "uart.h"
#include <stdint.h>

typedef struct
{
    uint32_t irq_count;
    uint32_t tick;
} msg_t;

static hr_queue_t q;
static msg_t storage[4];
static hr_task_t consumer, idle;

static uint32_t cs[160] __attribute__((aligned(8))), is[96] __attribute__((aligned(8)));

volatile uint32_t received;

static void ct(void *a)
{
    (void) a;
    msg_t m;
    for (;;)
        if (hr_queue_receive(&q, &m, HR_WAIT_FOREVER) == HR_WAIT_SUCCESS)
        {
            received = m.irq_count;
            gpio_led_toggle();
        }
}

static void it(void *a)
{
    (void) a;
    for (;;)
    {
        hr_port_wait_for_interrupt();
    }
}

void EXTI0_IRQHandler(void)
{
    static uint32_t n;
    bool higher = false;
    msg_t m =
    {
        ++n,
        hr_scheduler_tick_now()
    };
    EXTI_PR = 1UL;
    (void) hr_queue_send_from_isr(&q, &m, &higher);
}

int main(void)
{
    clock_init_hsi_8mhz();
    gpio_led_init();
    gpio_button_exti0_init();
    uart1_init_9600_hsi8();
    hr_scheduler_init();
    HR_ASSERT(hr_queue_init_static(&q, "irq-q", storage, 4, sizeof(msg_t)));
    HR_ASSERT(hr_task_create_static(&consumer, "consumer", 0, 0, ct, 0, cs, 160));
    HR_ASSERT(hr_task_create_static(&idle, "idle", 1, 3, it, 0, is, 96));
    HR_ASSERT(hr_scheduler_add_task(&consumer));
    HR_ASSERT(hr_scheduler_add_task(&idle));
    systick_init_1khz();
    hr_scheduler_start();
}
