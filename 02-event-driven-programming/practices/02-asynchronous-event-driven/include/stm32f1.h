#ifndef STM32F1_H
#define STM32F1_H

#include <stdint.h>

#define REG32(address) \
    (*(volatile uint32_t *)(uintptr_t)(address))

#define RCC_BASE_ADDRESS       (0x40021000UL)
#define AFIO_BASE_ADDRESS      (0x40010000UL)
#define EXTI_BASE_ADDRESS      (0x40010400UL)
#define GPIOA_BASE_ADDRESS     (0x40010800UL)
#define GPIOC_BASE_ADDRESS     (0x40011000UL)
#define USART1_BASE_ADDRESS    (0x40013800UL)

#define RCC_APB2ENR            REG32(RCC_BASE_ADDRESS + 0x18UL)

#define AFIO_EXTICR1           REG32(AFIO_BASE_ADDRESS + 0x08UL)

#define EXTI_IMR               REG32(EXTI_BASE_ADDRESS + 0x00UL)
#define EXTI_RTSR              REG32(EXTI_BASE_ADDRESS + 0x08UL)
#define EXTI_FTSR              REG32(EXTI_BASE_ADDRESS + 0x0CUL)
#define EXTI_PR                REG32(EXTI_BASE_ADDRESS + 0x14UL)

#define GPIOA_CRL              REG32(GPIOA_BASE_ADDRESS + 0x00UL)
#define GPIOA_CRH              REG32(GPIOA_BASE_ADDRESS + 0x04UL)
#define GPIOA_IDR              REG32(GPIOA_BASE_ADDRESS + 0x08UL)
#define GPIOA_ODR              REG32(GPIOA_BASE_ADDRESS + 0x0CUL)

#define GPIOC_CRH              REG32(GPIOC_BASE_ADDRESS + 0x04UL)
#define GPIOC_ODR              REG32(GPIOC_BASE_ADDRESS + 0x0CUL)
#define GPIOC_BSRR             REG32(GPIOC_BASE_ADDRESS + 0x10UL)

#define USART1_SR              REG32(USART1_BASE_ADDRESS + 0x00UL)
#define USART1_DR              REG32(USART1_BASE_ADDRESS + 0x04UL)
#define USART1_BRR             REG32(USART1_BASE_ADDRESS + 0x08UL)
#define USART1_CR1             REG32(USART1_BASE_ADDRESS + 0x0CUL)
#define USART1_CR2             REG32(USART1_BASE_ADDRESS + 0x10UL)
#define USART1_CR3             REG32(USART1_BASE_ADDRESS + 0x14UL)

#define SYSTICK_CTRL           REG32(0xE000E010UL)
#define SYSTICK_LOAD           REG32(0xE000E014UL)
#define SYSTICK_VAL            REG32(0xE000E018UL)

#define NVIC_ISER0             REG32(0xE000E100UL)
#define NVIC_ISER1             REG32(0xE000E104UL)

#endif
