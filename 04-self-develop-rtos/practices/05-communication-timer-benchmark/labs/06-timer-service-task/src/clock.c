#include "clock.h"
#include "stm32f1.h"

void clock_init_hsi_8mhz(void)
{
    RCC_CR |= RCC_CR_HSION;
    while ((RCC_CR & RCC_CR_HSIRDY) == 0U)
    {
    }

    RCC_CFGR &= ~0x3UL;
    while ((RCC_CFGR & (0x3UL << 2)) != 0U)
    {
    }
}
