#include "ak_port.h"
#include "stm32f1.h"

#define SCB_AIRCR REG32(0xE000ED0CUL)

void ak_port_reset(void)
{
    __asm volatile("dsb" : : : "memory");
    SCB_AIRCR = (0x5FAUL << 16U) | (1UL << 2U);
    __asm volatile("dsb" : : : "memory");
    for (;;)
    {
        __asm volatile("nop");
    }
}
