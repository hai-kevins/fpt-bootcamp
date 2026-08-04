#include "uart.h"

#include <stddef.h>
#include <stdint.h>

#define REG32(address) (*(volatile uint32_t *)(uintptr_t)(address))

#define RCC_BASE_ADDRESS    (0x40021000UL)
#define GPIOA_BASE_ADDRESS  (0x40010800UL)
#define USART1_BASE_ADDRESS (0x40013800UL)

#define RCC_APB2ENR REG32(RCC_BASE_ADDRESS + 0x18UL)
#define GPIOA_CRH   REG32(GPIOA_BASE_ADDRESS + 0x04UL)

#define USART1_SR  REG32(USART1_BASE_ADDRESS + 0x00UL)
#define USART1_DR  REG32(USART1_BASE_ADDRESS + 0x04UL)
#define USART1_BRR REG32(USART1_BASE_ADDRESS + 0x08UL)
#define USART1_CR1 REG32(USART1_BASE_ADDRESS + 0x0CUL)
#define USART1_CR2 REG32(USART1_BASE_ADDRESS + 0x10UL)
#define USART1_CR3 REG32(USART1_BASE_ADDRESS + 0x14UL)

#define RCC_APB2ENR_AFIOEN   (1UL << 0U)
#define RCC_APB2ENR_IOPAEN   (1UL << 2U)
#define RCC_APB2ENR_USART1EN (1UL << 14U)

#define USART_SR_RXNE (1UL << 5U)
#define USART_SR_TXE  (1UL << 7U)

#define USART_CR1_RE (1UL << 2U)
#define USART_CR1_TE (1UL << 3U)
#define USART_CR1_UE (1UL << 13U)

#define GPIO_NIBBLE_MASK    (0xFUL)
#define GPIO_AF_PP_2MHZ     (0xAUL)
#define GPIO_INPUT_FLOATING (0x4UL)

bool uart1_init(uint32_t peripheral_clock_hz, uint32_t baud_rate)
{
    uint32_t baud_divisor;

    if ((peripheral_clock_hz == 0UL) || (baud_rate == 0UL))
    {
        return false;
    }

    RCC_APB2ENR |= RCC_APB2ENR_AFIOEN | RCC_APB2ENR_IOPAEN | RCC_APB2ENR_USART1EN;

    /* PA9 = alternate-function push-pull; PA10 = floating input. */ GPIOA_CRH = (GPIOA_CRH & ~((GPIO_NIBBLE_MASK << 4U) | (GPIO_NIBBLE_MASK << 8U))) | (GPIO_AF_PP_2MHZ << 4U) | (GPIO_INPUT_FLOATING << 8U);

    USART1_CR1 = 0UL;
    USART1_CR2 = 0UL;
    USART1_CR3 = 0UL;

    baud_divisor = (peripheral_clock_hz + (baud_rate / 2UL)) / baud_rate;

    if ((baud_divisor == 0UL) || (baud_divisor > 0xFFFFUL))
    {
        return false;
    }

    USART1_BRR = baud_divisor;
    USART1_CR1 = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;

    return true;
}

void uart1_send_byte(uint8_t byte)
{
    while ((USART1_SR & USART_SR_TXE) == 0UL)
    {
    }

    USART1_DR = (uint32_t) byte;
}

void uart1_send_string(const char *text)
{
    if (text == NULL)
    {
        return;
    }

    while (*text != '\0')
    {
        uart1_send_byte((uint8_t)*text);
        text++;
    }
}

bool uart1_byte_available(void)
{
    return (USART1_SR & USART_SR_RXNE) != 0UL;
}

uint8_t uart1_read_byte(void)
{
    while (!uart1_byte_available())
    {
    }

    return (uint8_t)(USART1_DR & 0xFFUL);
}
