#include "systick.h"
#include "stm32f1.h"

static volatile uint32_t g_tick_count;

bool systick_init(uint32_t core_clock_hz, uint32_t tick_hz)
{
    uint32_t reload;

    if ((core_clock_hz == 0UL) || (tick_hz == 0UL))
    {
        return false;
    }

    reload = core_clock_hz / tick_hz;

    if ((reload == 0UL) || ((reload - 1UL) > SYSTICK_LOAD_MASK))
    {
        return false;
    }

    g_tick_count = 0UL;
    SYSTICK->CTRL = 0UL;
    SYSTICK->LOAD = reload - 1UL;
    SYSTICK->VAL = 0UL;
    SYSTICK->CTRL = SYSTICK_CTRL_CLKSOURCE | SYSTICK_CTRL_TICKINT | SYSTICK_CTRL_ENABLE;
    return true;
}

uint32_t systick_now(void)
{
    return g_tick_count;
}

void systick_delay_ms(uint32_t delay_ms)
{
    const uint32_t start = systick_now();

    while ((uint32_t)(systick_now() - start) < delay_ms)
    {
    }
}

void SysTick_Handler(void)
{
    g_tick_count++;
}
