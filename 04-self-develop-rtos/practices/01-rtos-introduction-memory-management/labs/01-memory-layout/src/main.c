#include "gpio.h"
#include "systick.h"
#include "uart.h"
#include <stdint.h>

extern uint32_t _etext;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;
extern uint32_t _end;
extern uint32_t _sstack;
extern uint32_t _estack;

const uint32_t g_const_value = 0x11111111UL;
uint32_t g_initialized_value = 0x22222222UL;
uint32_t g_zero_value;
static uint8_t g_buffer[128];

static void print_symbol(const char *name, const void *address)
{
    uart_write_string(name);
    uart_write_string(": ");
    uart_write_pointer(address);
    uart_write_string("\r\n");
}

int main(void)
{
    uart_init(9600UL, 8000000UL);
    (void) systick_init(8000000UL, 1000UL);
    gpio_enable_clock(GPIO_PORT_C);
    gpio_config_output_push_pull(GPIO_PORT_C, 13UL);
    gpio_write(GPIO_PORT_C, 13UL, true);

    uart_write_line("Lab 01 - MCU memory layout");
    print_symbol(".text end", &_etext);
    print_symbol(".data start", &_sdata);
    print_symbol(".data end", &_edata);
    print_symbol(".bss start", &_sbss);
    print_symbol(".bss end", &_ebss);
    print_symbol("static end", &_end);
    print_symbol("main stack low", &_sstack);
    print_symbol("main stack top", &_estack);
    print_symbol("g_const_value", &g_const_value);
    print_symbol("g_initialized_value", &g_initialized_value);
    print_symbol("g_zero_value", &g_zero_value);
    print_symbol("g_buffer", g_buffer);

    for (;;)
    {
        gpio_toggle(GPIO_PORT_C, 13UL);
        systick_delay_ms(500UL);
    }
}
