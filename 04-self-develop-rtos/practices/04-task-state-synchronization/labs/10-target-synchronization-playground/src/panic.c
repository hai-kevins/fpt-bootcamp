#include "panic.h"
#include "stm32f1.h"

volatile hr_panic_code_t g_hr_panic_code;
volatile uint32_t g_hr_panic_detail;

void hr_panic(hr_panic_code_t code, uint32_t detail)
{
    __asm volatile("cpsid i" ::: "memory");
    g_hr_panic_code = code;
    g_hr_panic_detail = detail;

    for (;;)
    {
        __asm volatile("nop");
    }
}

void hr_assert_failed(uint32_t line)
{
    hr_panic(HR_PANIC_ASSERT, line);
}

void HardFault_Handler(void)
{
    g_hr_panic_detail = SCB_CFSR ^ SCB_HFSR;
    hr_panic(HR_PANIC_HARDFAULT, g_hr_panic_detail);
}
