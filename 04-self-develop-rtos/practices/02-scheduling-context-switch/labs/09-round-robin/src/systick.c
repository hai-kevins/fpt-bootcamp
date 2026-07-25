#include "systick.h"
#include "scheduler.h"
#include "stm32f1.h"

void systick_init_1khz(void)
{
    SYSTICK_CTRL = 0U;
    SYSTICK_LOAD = 7999UL;
    SYSTICK_VAL = 0U;
    SYSTICK_CTRL = SYSTICK_CTRL_CLKSOURCE |
                   SYSTICK_CTRL_TICKINT |
                   SYSTICK_CTRL_ENABLE;
}

void systick_stop(void)
{
    SYSTICK_CTRL = 0U;
}

void SysTick_Handler(void)
{
    rtos_scheduler_on_tick();
}
