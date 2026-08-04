#include "gpio.h"
#include "systick.h"
#include "uart.h"
#include <stdint.h>

#define INITIAL_VALUE (0x13579BDFUL)

uint32_t g_initialized = INITIAL_VALUE;
uint32_t g_zero_initialized;

int main(void)
{
    const bool data_ok = g_initialized == INITIAL_VALUE;
    const bool bss_ok = g_zero_initialized == 0UL;

    uart_init(9600UL, 8000000UL);
    (void) systick_init(8000000UL, 1000UL);
    gpio_enable_clock(GPIO_PORT_C);
    gpio_config_output_push_pull(GPIO_PORT_C, 13UL);

    uart_write_line("Lab 02 - Startup sections");
    uart_write_string(".data copy: ");
    uart_write_line(data_ok ? "PASS" : "FAIL");
    uart_write_string(".bss clear: ");
    uart_write_line(bss_ok ? "PASS" : "FAIL");

    for (;;)
    {
        gpio_write(GPIO_PORT_C, 13UL, !(data_ok && bss_ok));
        systick_delay_ms(250UL);
        gpio_write(GPIO_PORT_C, 13UL, true);
        systick_delay_ms(750UL);
    }
}
