#include "compiler.h"
#include "stm32f1.h"
#include <stdint.h>

extern uint32_t _estack;
extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;

extern int main(void);
extern void SysTick_Handler(void);

void Reset_Handler(void);
void Default_Handler(void);
void NMI_Handler(void) ATTR_WEAK;
void HardFault_Handler(void) ATTR_WEAK;
void MemManage_Handler(void) ATTR_WEAK;
void BusFault_Handler(void) ATTR_WEAK;
void UsageFault_Handler(void) ATTR_WEAK;
void SVC_Handler(void) ATTR_WEAK;
void DebugMon_Handler(void) ATTR_WEAK;
void PendSV_Handler(void) ATTR_WEAK;

#define WEAK_IRQ(name) void name(void) ATTR_WEAK; void name(void) { Default_Handler(); }

WEAK_IRQ(WWDG_IRQHandler) WEAK_IRQ(PVD_IRQHandler) WEAK_IRQ(TAMPER_IRQHandler) WEAK_IRQ(RTC_IRQHandler) WEAK_IRQ(FLASH_IRQHandler) WEAK_IRQ(RCC_IRQHandler) WEAK_IRQ(EXTI0_IRQHandler) WEAK_IRQ(EXTI1_IRQHandler) WEAK_IRQ(EXTI2_IRQHandler) WEAK_IRQ(EXTI3_IRQHandler) WEAK_IRQ(EXTI4_IRQHandler) WEAK_IRQ(DMA1_Channel1_IRQHandler) WEAK_IRQ(DMA1_Channel2_IRQHandler) WEAK_IRQ(DMA1_Channel3_IRQHandler) WEAK_IRQ(DMA1_Channel4_IRQHandler) WEAK_IRQ(DMA1_Channel5_IRQHandler) WEAK_IRQ(DMA1_Channel6_IRQHandler) WEAK_IRQ(DMA1_Channel7_IRQHandler) WEAK_IRQ(ADC1_2_IRQHandler) WEAK_IRQ(USB_HP_CAN_TX_IRQHandler) WEAK_IRQ(USB_LP_CAN_RX0_IRQHandler) WEAK_IRQ(CAN_RX1_IRQHandler) WEAK_IRQ(CAN_SCE_IRQHandler) WEAK_IRQ(EXTI9_5_IRQHandler) WEAK_IRQ(TIM1_BRK_IRQHandler) WEAK_IRQ(TIM1_UP_IRQHandler) WEAK_IRQ(TIM1_TRG_COM_IRQHandler) WEAK_IRQ(TIM1_CC_IRQHandler) WEAK_IRQ(TIM2_IRQHandler) WEAK_IRQ(TIM3_IRQHandler) WEAK_IRQ(TIM4_IRQHandler) WEAK_IRQ(I2C1_EV_IRQHandler) WEAK_IRQ(I2C1_ER_IRQHandler) WEAK_IRQ(I2C2_EV_IRQHandler) WEAK_IRQ(I2C2_ER_IRQHandler) WEAK_IRQ(SPI1_IRQHandler) WEAK_IRQ(SPI2_IRQHandler) WEAK_IRQ(USART1_IRQHandler) WEAK_IRQ(USART2_IRQHandler) WEAK_IRQ(USART3_IRQHandler) WEAK_IRQ(EXTI15_10_IRQHandler) WEAK_IRQ(RTCAlarm_IRQHandler) WEAK_IRQ(USBWakeUp_IRQHandler)

ATTR_USED ATTR_SECTION(".isr_vector") void (*const g_vector_table[])(void) =
{
    (void (*)(void))(&_estack),
    Reset_Handler,
    NMI_Handler,
    HardFault_Handler,
    MemManage_Handler,
    BusFault_Handler,
    UsageFault_Handler,
    0,
    0,
    0,
    0,
    SVC_Handler,
    DebugMon_Handler,
    0,
    PendSV_Handler,
    SysTick_Handler,
    WWDG_IRQHandler,
    PVD_IRQHandler,
    TAMPER_IRQHandler,
    RTC_IRQHandler,
    FLASH_IRQHandler,
    RCC_IRQHandler,
    EXTI0_IRQHandler,
    EXTI1_IRQHandler,
    EXTI2_IRQHandler,
    EXTI3_IRQHandler,
    EXTI4_IRQHandler,
    DMA1_Channel1_IRQHandler,
    DMA1_Channel2_IRQHandler,
    DMA1_Channel3_IRQHandler,
    DMA1_Channel4_IRQHandler,
    DMA1_Channel5_IRQHandler,
    DMA1_Channel6_IRQHandler,
    DMA1_Channel7_IRQHandler,
    ADC1_2_IRQHandler,
    USB_HP_CAN_TX_IRQHandler,
    USB_LP_CAN_RX0_IRQHandler,
    CAN_RX1_IRQHandler,
    CAN_SCE_IRQHandler,
    EXTI9_5_IRQHandler,
    TIM1_BRK_IRQHandler,
    TIM1_UP_IRQHandler,
    TIM1_TRG_COM_IRQHandler,
    TIM1_CC_IRQHandler,
    TIM2_IRQHandler,
    TIM3_IRQHandler,
    TIM4_IRQHandler,
    I2C1_EV_IRQHandler,
    I2C1_ER_IRQHandler,
    I2C2_EV_IRQHandler,
    I2C2_ER_IRQHandler,
    SPI1_IRQHandler,
    SPI2_IRQHandler,
    USART1_IRQHandler,
    USART2_IRQHandler,
    USART3_IRQHandler,
    EXTI15_10_IRQHandler,
    RTCAlarm_IRQHandler,
    USBWakeUp_IRQHandler
};

ATTR_NORETURN void Reset_Handler(void)
{
    uint32_t *source = &_sidata;
    uint32_t *destination = &_sdata;

    while (destination < &_edata)
    {
        *destination++ = *source++;
    }

    destination = &_sbss;
    while (destination < &_ebss)
    {
        *destination++ = 0UL;
    }

    (void) main();

    for (;;)
    {
        cpu_wait_for_interrupt();
    }
}

ATTR_NORETURN void Default_Handler(void)
{
    cpu_disable_irq();
    cpu_breakpoint();

    for (;;)
    {
        cpu_wait_for_interrupt();
    }
}

void NMI_Handler(void)
{
    Default_Handler();
}

void HardFault_Handler(void)
{
    Default_Handler();
}

void MemManage_Handler(void)
{
    Default_Handler();
}

void BusFault_Handler(void)
{
    Default_Handler();
}

void UsageFault_Handler(void)
{
    Default_Handler();
}

void SVC_Handler(void)
{
    Default_Handler();
}

void DebugMon_Handler(void)
{
    Default_Handler();
}

void PendSV_Handler(void)
{
    Default_Handler();
}
