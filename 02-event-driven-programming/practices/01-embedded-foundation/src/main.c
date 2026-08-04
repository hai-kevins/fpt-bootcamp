#include "gpio.h"
#include "uart.h"

#include <stdint.h>

#define HSI_CLOCK_HZ   (8000000UL)
#define UART_BAUD_RATE (9600UL)

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

    if (!uart1_init(HSI_CLOCK_HZ, UART_BAUD_RATE))
    {
        for (;;)
        {
            gpio_led_toggle();
            delay_cycles(50000UL);
        }
    }

    uart1_send_string("\r\n""STM32F103 bare-metal foundation\r\n""Clock: HSI 8 MHz\r\n""USART1: 9600 8-N-1\r\n""Type characters to test echo.\r\n");

    for (;;)
    {
        gpio_led_toggle();
        uart1_send_string("tick\r\n");

        for (uint32_t i = 0UL; i < 20UL; i++)
        {
            delay_cycles(50000UL);

            while (uart1_byte_available())
            {
                const uint8_t byte = uart1_read_byte();
                uart1_send_byte(byte);
            }
        }
    }
}
