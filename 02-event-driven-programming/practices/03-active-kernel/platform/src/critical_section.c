#include "ak_port.h"

uint32_t ak_port_critical_enter(void)
{
    uint32_t state;
    __asm volatile (
        "mrs %0, primask\n"
        "cpsid i\n"
        : "=r" (state)
        :
        : "memory"
    );
    return state;
}

void ak_port_critical_exit(uint32_t previous_state)
{
    if ((previous_state & 1U) == 0U)
    {
        __asm volatile ("cpsie i" ::: "memory");
    }
}
