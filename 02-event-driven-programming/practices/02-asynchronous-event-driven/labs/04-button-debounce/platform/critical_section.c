#include "platform.h"

uint32_t platform_critical_enter(void)
{
    uint32_t state;

    __asm volatile("mrs %0, primask\n""cpsid i\n" : "=r"(state) : : "memory");

    return state;
}

void platform_critical_exit(uint32_t previous_state)
{
    if ((previous_state & 1UL) == 0UL)
    {
        __asm volatile("cpsie i" : : : "memory");
    }
}
