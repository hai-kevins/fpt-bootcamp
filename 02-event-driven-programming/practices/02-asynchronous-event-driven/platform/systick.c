#include "platform.h"
#include "stm32f1.h"

static volatile uint32_t g_tick_ms;

static void systick_init(void)
{
    g_tick_ms = 0UL;

    SYSTICK_LOAD = 7999UL;
    SYSTICK_VAL = 0UL;
    SYSTICK_CTRL =
        (1UL << 2U) |
        (1UL << 1U) |
        (1UL << 0U);
}

uint32_t platform_time_now_ms(void)
{
    return g_tick_ms;
}

void SysTick_Handler(void)
{
    g_tick_ms++;
}

void platform_systick_start(void)
{
    systick_init();
}
