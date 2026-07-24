#include <stdint.h>

extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;
extern uint32_t _estack;

int main(void);

void Reset_Handler(void) __attribute__((noreturn));
void Default_Handler(void) __attribute__((noreturn));

#define WEAK_DEFAULT_HANDLER(name) \
    void name(void) __attribute__((weak, alias("Default_Handler")))

WEAK_DEFAULT_HANDLER(NMI_Handler);
WEAK_DEFAULT_HANDLER(HardFault_Handler);
WEAK_DEFAULT_HANDLER(MemManage_Handler);
WEAK_DEFAULT_HANDLER(BusFault_Handler);
WEAK_DEFAULT_HANDLER(UsageFault_Handler);
WEAK_DEFAULT_HANDLER(SVC_Handler);
WEAK_DEFAULT_HANDLER(DebugMon_Handler);
WEAK_DEFAULT_HANDLER(PendSV_Handler);
WEAK_DEFAULT_HANDLER(SysTick_Handler);
WEAK_DEFAULT_HANDLER(WWDG_IRQHandler);
WEAK_DEFAULT_HANDLER(PVD_IRQHandler);
WEAK_DEFAULT_HANDLER(TAMPER_IRQHandler);
WEAK_DEFAULT_HANDLER(RTC_IRQHandler);
WEAK_DEFAULT_HANDLER(FLASH_IRQHandler);
WEAK_DEFAULT_HANDLER(RCC_IRQHandler);
WEAK_DEFAULT_HANDLER(EXTI0_IRQHandler);
WEAK_DEFAULT_HANDLER(EXTI1_IRQHandler);
WEAK_DEFAULT_HANDLER(EXTI2_IRQHandler);
WEAK_DEFAULT_HANDLER(EXTI3_IRQHandler);
WEAK_DEFAULT_HANDLER(EXTI4_IRQHandler);
WEAK_DEFAULT_HANDLER(DMA1_Channel1_IRQHandler);
WEAK_DEFAULT_HANDLER(DMA1_Channel2_IRQHandler);
WEAK_DEFAULT_HANDLER(DMA1_Channel3_IRQHandler);
WEAK_DEFAULT_HANDLER(DMA1_Channel4_IRQHandler);
WEAK_DEFAULT_HANDLER(DMA1_Channel5_IRQHandler);
WEAK_DEFAULT_HANDLER(DMA1_Channel6_IRQHandler);
WEAK_DEFAULT_HANDLER(DMA1_Channel7_IRQHandler);
WEAK_DEFAULT_HANDLER(ADC1_2_IRQHandler);
WEAK_DEFAULT_HANDLER(USB_HP_CAN1_TX_IRQHandler);
WEAK_DEFAULT_HANDLER(USB_LP_CAN1_RX0_IRQHandler);
WEAK_DEFAULT_HANDLER(CAN1_RX1_IRQHandler);
WEAK_DEFAULT_HANDLER(CAN1_SCE_IRQHandler);
WEAK_DEFAULT_HANDLER(EXTI9_5_IRQHandler);
WEAK_DEFAULT_HANDLER(TIM1_BRK_IRQHandler);
WEAK_DEFAULT_HANDLER(TIM1_UP_IRQHandler);
WEAK_DEFAULT_HANDLER(TIM1_TRG_COM_IRQHandler);
WEAK_DEFAULT_HANDLER(TIM1_CC_IRQHandler);
WEAK_DEFAULT_HANDLER(TIM2_IRQHandler);
WEAK_DEFAULT_HANDLER(TIM3_IRQHandler);
WEAK_DEFAULT_HANDLER(TIM4_IRQHandler);
WEAK_DEFAULT_HANDLER(I2C1_EV_IRQHandler);
WEAK_DEFAULT_HANDLER(I2C1_ER_IRQHandler);
WEAK_DEFAULT_HANDLER(I2C2_EV_IRQHandler);
WEAK_DEFAULT_HANDLER(I2C2_ER_IRQHandler);
WEAK_DEFAULT_HANDLER(SPI1_IRQHandler);
WEAK_DEFAULT_HANDLER(SPI2_IRQHandler);
WEAK_DEFAULT_HANDLER(USART1_IRQHandler);
WEAK_DEFAULT_HANDLER(USART2_IRQHandler);
WEAK_DEFAULT_HANDLER(USART3_IRQHandler);
WEAK_DEFAULT_HANDLER(EXTI15_10_IRQHandler);
WEAK_DEFAULT_HANDLER(RTCAlarm_IRQHandler);
WEAK_DEFAULT_HANDLER(USBWakeUp_IRQHandler);

typedef void (*isr_handler_t)(void);

__attribute__((used, section(".isr_vector"), aligned(256)))
const isr_handler_t g_vector_table[] =
{
    (isr_handler_t)(uintptr_t)&_estack,
    Reset_Handler,
    NMI_Handler,
    HardFault_Handler,
    MemManage_Handler,
    BusFault_Handler,
    UsageFault_Handler,
    (isr_handler_t)0,
    (isr_handler_t)0,
    (isr_handler_t)0,
    (isr_handler_t)0,
    SVC_Handler,
    DebugMon_Handler,
    (isr_handler_t)0,
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
    USB_HP_CAN1_TX_IRQHandler,
    USB_LP_CAN1_RX0_IRQHandler,
    CAN1_RX1_IRQHandler,
    CAN1_SCE_IRQHandler,
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

void Reset_Handler(void)
{
    uint32_t *source = &_sidata;
    uint32_t *destination = &_sdata;

    while (destination < &_edata)
    {
        *destination = *source;
        ++destination;
        ++source;
    }

    destination = &_sbss;
    while (destination < &_ebss)
    {
        *destination = 0U;
        ++destination;
    }

    (void)main();

    for (;;)
    {
        __asm volatile("nop");
    }
}

void Default_Handler(void)
{
    __asm volatile("cpsid i" ::: "memory");
    for (;;)
    {
        __asm volatile("nop");
    }
}
