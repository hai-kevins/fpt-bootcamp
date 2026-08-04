#include "gpio.h"
#include "uart.h"

#include <stdint.h>

#define HSI_CLOCK_HZ   (8000000UL)
#define BAUD_RATE      (9600UL)

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

    if (!uart1_init(HSI_CLOCK_HZ, BAUD_RATE))
    {
        for (;;)
        {
            gpio_led_toggle();
            delay_cycles(100000UL);
        }
    }

    uart1_send_string("\r\nLab 07 - USART1 polling\r\n""PA9=TX, PA10=RX, 9600 8-N-1\r\n""Every received byte is echoed.\r\n");

    for (;;)
    {
        if (uart1_byte_available())
        {
            const uint8_t byte = uart1_read_byte();

            uart1_send_string("RX: ");
            uart1_send_byte(byte);
            uart1_send_string("\r\n");

            gpio_led_toggle();
        }
    }
}
