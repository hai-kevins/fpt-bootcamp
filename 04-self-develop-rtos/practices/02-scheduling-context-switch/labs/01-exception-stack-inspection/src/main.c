#include "clock.h"
#include "uart.h"

#include <stdint.h>

volatile uint32_t g_svc_msp;
volatile uint32_t g_svc_psp;
volatile uint32_t g_svc_exc_return;
volatile uint32_t g_svc_frame[8];

static uint32_t g_process_stack[64] __attribute__((aligned(8)));

void lab_svc_capture(uint32_t *stacked, uint32_t psp, uint32_t exc_return)
{
    g_svc_msp = (uint32_t)(uintptr_t) stacked;
    g_svc_psp = psp;
    g_svc_exc_return = exc_return;

    for (uint32_t index = 0U; index < 8U; ++index)
    {
        g_svc_frame[index] = stacked[index];
    }
}

static void print_value(const char *name, uint32_t value)
{
    uart1_write_string(name);
    uart1_write_hex32(value);
    uart1_write_string("\r\n");
}

int main(void)
{
    const uint32_t psp_top = (uint32_t)(uintptr_t)(g_process_stack + 64U);

    clock_init_hsi_8mhz();
    uart1_init_9600_hsi8();

    __asm volatile("msr psp, %0" : : "r"(psp_top) : "memory");
    __asm volatile("svc 0");

    uart1_write_string("\r\nLab 01 exception frame\r\n");
    print_value("MSP frame: ", g_svc_msp);
    print_value("PSP:       ", g_svc_psp);
    print_value("EXC_RETURN:", g_svc_exc_return);

    for (uint32_t index = 0U; index < 8U; ++index)
    {
        uart1_write_string("frame[");
        uart1_write_u32(index);
        uart1_write_string("]=");
        uart1_write_hex32(g_svc_frame[index]);
        uart1_write_string("\r\n");
    }

    for (;;)
    {
        __asm volatile("wfi");
    }
}
