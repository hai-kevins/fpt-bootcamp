#ifndef PLATFORM_STM32F1_H
#define PLATFORM_STM32F1_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define PERIPH_BASE              (0x40000000UL)
#define APB2_BASE                (0x40010000UL)
#define AHB_BASE                 (0x40018000UL)

#define GPIOA_BASE               (APB2_BASE + 0x0800UL)
#define GPIOB_BASE               (APB2_BASE + 0x0C00UL)
#define GPIOC_BASE               (APB2_BASE + 0x1000UL)
#define USART1_BASE              (APB2_BASE + 0x3800UL)
#define RCC_BASE                 (AHB_BASE + 0x9000UL)
#define FLASH_IF_BASE            (0x40022000UL)
#define SYSTICK_BASE             (0xE000E010UL)
#define SCB_BASE                 (0xE000ED00UL)

#define MMIO32(address) (*(volatile uint32_t *)(uintptr_t)(address))

typedef struct
{
    volatile uint32_t CRL;
    volatile uint32_t CRH;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t BRR;
    volatile uint32_t LCKR;
} stm32_gpio_t;

typedef struct
{
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t BRR;
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t GTPR;
} stm32_usart_t;

typedef struct
{
    volatile uint32_t CR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t APB2RSTR;
    volatile uint32_t APB1RSTR;
    volatile uint32_t AHBENR;
    volatile uint32_t APB2ENR;
    volatile uint32_t APB1ENR;
    volatile uint32_t BDCR;
    volatile uint32_t CSR;
} stm32_rcc_t;

typedef struct
{
    volatile uint32_t ACR;
    volatile uint32_t KEYR;
    volatile uint32_t OPTKEYR;
    volatile uint32_t SR;
    volatile uint32_t CR;
    volatile uint32_t AR;
    volatile uint32_t RESERVED;
    volatile uint32_t OBR;
    volatile uint32_t WRPR;
} stm32_flash_t;

typedef struct
{
    volatile uint32_t CTRL;
    volatile uint32_t LOAD;
    volatile uint32_t VAL;
    volatile uint32_t CALIB;
} stm32_systick_t;

typedef struct
{
    volatile uint32_t CPUID;
    volatile uint32_t ICSR;
    volatile uint32_t VTOR;
    volatile uint32_t AIRCR;
    volatile uint32_t SCR;
    volatile uint32_t CCR;
    volatile uint8_t SHP[12];
    volatile uint32_t SHCSR;
} stm32_scb_t;

#define GPIOA ((stm32_gpio_t *)GPIOA_BASE)
#define GPIOB ((stm32_gpio_t *)GPIOB_BASE)
#define GPIOC ((stm32_gpio_t *)GPIOC_BASE)
#define USART1 ((stm32_usart_t *)USART1_BASE)
#define RCC ((stm32_rcc_t *)RCC_BASE)
#define FLASH_IF ((stm32_flash_t *)FLASH_IF_BASE)
#define SYSTICK ((stm32_systick_t *)SYSTICK_BASE)
#define SCB ((stm32_scb_t *)SCB_BASE)

#define RCC_CR_HSEON              (1UL << 16)
#define RCC_CR_HSERDY             (1UL << 17)
#define RCC_CR_PLLON              (1UL << 24)
#define RCC_CR_PLLRDY             (1UL << 25)

#define RCC_CFGR_SW_MASK          (3UL << 0)
#define RCC_CFGR_SW_PLL           (2UL << 0)
#define RCC_CFGR_SWS_MASK         (3UL << 2)
#define RCC_CFGR_SWS_PLL          (2UL << 2)
#define RCC_CFGR_HPRE_DIV1        (0UL << 4)
#define RCC_CFGR_PPRE1_DIV2       (4UL << 8)
#define RCC_CFGR_PPRE2_DIV1       (0UL << 11)
#define RCC_CFGR_ADCPRE_DIV6      (2UL << 14)
#define RCC_CFGR_PLLSRC_HSE       (1UL << 16)
#define RCC_CFGR_PLLMULL9         (7UL << 18)

#define RCC_APB2ENR_AFIOEN        (1UL << 0)
#define RCC_APB2ENR_IOPAEN        (1UL << 2)
#define RCC_APB2ENR_IOPBEN        (1UL << 3)
#define RCC_APB2ENR_IOPCEN        (1UL << 4)
#define RCC_APB2ENR_USART1EN      (1UL << 14)

#define FLASH_ACR_LATENCY_2       (2UL << 0)
#define FLASH_ACR_PRFTBE          (1UL << 4)

#define USART_SR_RXNE             (1UL << 5)
#define USART_SR_TXE              (1UL << 7)
#define USART_CR1_RE              (1UL << 2)
#define USART_CR1_TE              (1UL << 3)
#define USART_CR1_UE              (1UL << 13)

#define SYSTICK_CTRL_ENABLE       (1UL << 0)
#define SYSTICK_CTRL_TICKINT      (1UL << 1)
#define SYSTICK_CTRL_CLKSOURCE    (1UL << 2)
#define SYSTICK_LOAD_MASK         (0x00FFFFFFUL)

static inline void cpu_disable_irq(void)
{
    __asm volatile("cpsid i" : : : "memory");
}

static inline void cpu_enable_irq(void)
{
    __asm volatile("cpsie i" : : : "memory");
}

static inline void cpu_wait_for_interrupt(void)
{
    __asm volatile("wfi" : : : "memory");
}

static inline void cpu_breakpoint(void)
{
    __asm volatile("bkpt #0");
}

static inline uintptr_t cpu_get_msp(void)
{
    uintptr_t value;
    __asm volatile("mrs %0, msp" : "=r"(value));
    return value;
}

#endif
