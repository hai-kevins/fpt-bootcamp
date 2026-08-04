#include "gpio.h"

#include <stdint.h>

static void delay_cycles(volatile uint32_t cycles)
{
    while (cycles > 0UL)
    {
        __asm volatile("nop" : : : "memory");
        cycles--;
    }
}

int main(void)
{
    gpio_led_init();

    for (;;)
    {
        gpio_led_toggle();
        delay_cycles(800000UL);
    }
}
