#include "benchmark_clock.h"
#include "clock.h"
#include "gpio.h"
#include "stm32f1.h"
#include "uart.h"
#include <stdint.h>

int main(void)
{
    clock_init_hsi_8mhz();
    gpio_led_init();
    uart1_init_9600_hsi8();
    hr_benchmark_clock_init();
    uart1_write_string("DWT available=");
    uart1_write_u32(hr_benchmark_clock_available() ? 1U : 0U);
    uart1_write_string(" overhead=");
    uart1_write_u32(hr_benchmark_timestamp_overhead());
    uart1_write_string(" cycles\r\n");
    for (;;)
    {
        uint32_t s = hr_benchmark_now();
        gpio_led_on();
        gpio_led_off();
        uint32_t e = hr_benchmark_now();
        uart1_write_string("GPIO pair cycles=");
        uart1_write_u32(e - s);
        uart1_write_string("\r\n");
        for (volatile uint32_t i = 0; i < 400000; i++)
        {
            __asm volatile("nop");
        }
    }
}
