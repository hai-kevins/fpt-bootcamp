#include "platform.h"

void platform_init(void)
{
    platform_led_init();
    platform_button_init();
    platform_uart_init();
    platform_systick_init();
}

void platform_idle(void)
{
    __asm volatile ("wfi" ::: "memory");
}
