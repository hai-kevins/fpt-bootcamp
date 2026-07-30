#include "ak_port.h"

__attribute__((weak))
uint32_t ak_port_critical_enter(void)
{
    return 0U;
}

__attribute__((weak))
void ak_port_critical_exit(uint32_t previous_state)
{
    (void)previous_state;
}

__attribute__((weak))
uint32_t ak_port_time_now_ms(void)
{
    return 0U;
}

__attribute__((weak))
void ak_port_reset(void)
{
}
