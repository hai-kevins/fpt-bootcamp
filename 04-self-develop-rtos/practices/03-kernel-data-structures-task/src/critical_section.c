#include "critical_section.h"

rtos_irq_state_t rtos_critical_enter(void)
{
    rtos_irq_state_t state;

    __asm volatile("mrs %0, primask\n""cpsid i\n" : "=r"(state) : : "memory");

    return state;
}

void rtos_critical_exit(rtos_irq_state_t state)
{
    __asm volatile("msr primask, %0\n" : : "r"(state) : "memory");
}
