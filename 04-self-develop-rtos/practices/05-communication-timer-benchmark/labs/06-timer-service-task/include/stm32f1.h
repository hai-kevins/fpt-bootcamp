#ifndef STM32F1_H
#define STM32F1_H

#include <stdint.h>

#define MMIO32(address) (*(volatile uint32_t *)(uintptr_t)(address))
#define MMIO8(address)  (*(volatile uint8_t *)(uintptr_t)(address))

#define RCC_BASE        (0x40021000UL)
#define RCC_CR          MMIO32(RCC_BASE + 0x00UL)
#define RCC_CFGR        MMIO32(RCC_BASE + 0x04UL)
#define RCC_APB2ENR     MMIO32(RCC_BASE + 0x18UL)

#define FLASH_BASE      (0x40022000UL)
#define FLASH_ACR       MMIO32(FLASH_BASE + 0x00UL)

#define AFIO_BASE       (0x40010000UL)
#define AFIO_EXTICR1    MMIO32(AFIO_BASE + 0x08UL)

#define EXTI_BASE       (0x40010400UL)
#define EXTI_IMR        MMIO32(EXTI_BASE + 0x00UL)
#define EXTI_RTSR       MMIO32(EXTI_BASE + 0x08UL)
#define EXTI_FTSR       MMIO32(EXTI_BASE + 0x0CUL)
#define EXTI_PR         MMIO32(EXTI_BASE + 0x14UL)

#define GPIOA_BASE      (0x40010800UL)
#define GPIOA_CRL       MMIO32(GPIOA_BASE + 0x00UL)
#define GPIOA_CRH       MMIO32(GPIOA_BASE + 0x04UL)
#define GPIOA_IDR       MMIO32(GPIOA_BASE + 0x08UL)
#define GPIOA_ODR       MMIO32(GPIOA_BASE + 0x0CUL)
#define GPIOA_BSRR      MMIO32(GPIOA_BASE + 0x10UL)
#define GPIOA_BRR       MMIO32(GPIOA_BASE + 0x14UL)

#define GPIOC_BASE      (0x40011000UL)
#define GPIOC_CRH       MMIO32(GPIOC_BASE + 0x04UL)
#define GPIOC_ODR       MMIO32(GPIOC_BASE + 0x0CUL)
#define GPIOC_BSRR      MMIO32(GPIOC_BASE + 0x10UL)
#define GPIOC_BRR       MMIO32(GPIOC_BASE + 0x14UL)

#define USART1_BASE     (0x40013800UL)
#define USART1_SR       MMIO32(USART1_BASE + 0x00UL)
#define USART1_DR       MMIO32(USART1_BASE + 0x04UL)
#define USART1_BRR      MMIO32(USART1_BASE + 0x08UL)
#define USART1_CR1      MMIO32(USART1_BASE + 0x0CUL)

#define SYSTICK_CTRL    MMIO32(0xE000E010UL)
#define SYSTICK_LOAD    MMIO32(0xE000E014UL)
#define SYSTICK_VAL     MMIO32(0xE000E018UL)

#define SCB_ICSR        MMIO32(0xE000ED04UL)
#define SCB_SHPR_SVC    MMIO8(0xE000ED1FUL)
#define SCB_SHPR_PENDSV MMIO8(0xE000ED22UL)
#define SCB_SHPR_SYSTICK MMIO8(0xE000ED23UL)
#define SCB_CFSR        MMIO32(0xE000ED28UL)
#define SCB_HFSR        MMIO32(0xE000ED2CUL)

#define NVIC_ISER0      MMIO32(0xE000E100UL)
#define NVIC_IPR0       MMIO8(0xE000E400UL)

#define RCC_CR_HSION    (1UL << 0)
#define RCC_CR_HSIRDY   (1UL << 1)
#define RCC_APB2ENR_AFIOEN  (1UL << 0)
#define RCC_APB2ENR_IOPAEN  (1UL << 2)
#define RCC_APB2ENR_IOPCEN  (1UL << 4)
#define RCC_APB2ENR_USART1EN (1UL << 14)

#define USART_SR_RXNE   (1UL << 5)
#define USART_SR_TXE    (1UL << 7)
#define USART_CR1_RE    (1UL << 2)
#define USART_CR1_TE    (1UL << 3)
#define USART_CR1_UE    (1UL << 13)

#define SCB_ICSR_PENDSVSET (1UL << 28)

#define SYSTICK_CTRL_ENABLE    (1UL << 0)
#define SYSTICK_CTRL_TICKINT   (1UL << 1)
#define SYSTICK_CTRL_CLKSOURCE (1UL << 2)

#endif
