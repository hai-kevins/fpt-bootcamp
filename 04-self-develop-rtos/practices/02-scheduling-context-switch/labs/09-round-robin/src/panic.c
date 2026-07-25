#include "panic.h"
#include "stm32f1.h"

volatile rtos_panic_code_t g_rtos_panic_code;
volatile uint32_t g_rtos_panic_detail;

void rtos_panic(rtos_panic_code_t code, uint32_t detail)
{
    __asm volatile("cpsid i" ::: "memory");
    g_rtos_panic_code = code;
    g_rtos_panic_detail = detail;

    for (;;)
    {
        __asm volatile("nop");
    }
}

void rtos_assert_failed(uint32_t line)
{
    rtos_panic(RTOS_PANIC_ASSERT, line);
}

void HardFault_Handler(void)
{
    g_rtos_panic_detail = SCB_CFSR ^ SCB_HFSR;
    rtos_panic(RTOS_PANIC_HARDFAULT, g_rtos_panic_detail);
}
