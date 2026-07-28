#include "cortex_m3_port.h"
#include "stm32f1.h"

void hr_port_init(void)
{
    SCB_SHPR_SVC = 0x80U;
    SCB_SHPR_SYSTICK = 0xA0U;
    SCB_SHPR_PENDSV = 0xF0U;
}

void hr_port_start_first_task(void)
{
    __asm volatile("svc 0");
    for (;;)
    {
    }
}

void hr_port_request_context_switch(void)
{
    SCB_ICSR = SCB_ICSR_PENDSVSET;
    __asm volatile("dsb" ::: "memory");
    __asm volatile("isb" ::: "memory");
}

void hr_port_wait_for_interrupt(void)
{
    __asm volatile("wfi" ::: "memory");
}

uint32_t hr_port_get_psp(void)
{
    uint32_t value;
    __asm volatile("mrs %0, psp" : "=r"(value));
    return value;
}

uint32_t hr_port_get_msp(void)
{
    uint32_t value;
    __asm volatile("mrs %0, msp" : "=r"(value));
    return value;
}
