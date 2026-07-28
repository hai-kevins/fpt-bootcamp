#include "critical_section.h"

hr_irq_state_t hr_critical_enter(void)
{
    hr_irq_state_t state;

    __asm volatile(
        "mrs %0, primask\n"
        "cpsid i\n"
        : "=r"(state)
        :
        : "memory");

    return state;
}

void hr_critical_exit(hr_irq_state_t state)
{
    __asm volatile(
        "msr primask, %0\n"
        :
        : "r"(state)
        : "memory");
}
