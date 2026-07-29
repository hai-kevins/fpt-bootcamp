#include "platform.h"

void platform_gpio_init(void);
void platform_uart_start(void);
void platform_systick_start(void);

void platform_init(void)
{
    platform_gpio_init();
    platform_uart_start();
    platform_systick_start();
}

void platform_idle(void)
{
    __asm volatile ("wfi" ::: "memory");
}
