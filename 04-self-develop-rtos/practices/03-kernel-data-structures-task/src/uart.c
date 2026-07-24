#include "uart.h"
#include "stm32f1.h"

void uart1_init_9600_hsi8(void)
{
    RCC_APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_USART1EN;

    GPIOA_CRH &= ~((0xFUL << 4) | (0xFUL << 8));
    GPIOA_CRH |= (0xBUL << 4); /* PA9: AF push-pull, 50 MHz. */
    GPIOA_CRH |= (0x4UL << 8); /* PA10: floating input. */

    USART1_CR1 = 0U;
    USART1_BRR = 0x0341UL; /* 8 MHz / 9600 baud, oversampling by 16. */
    USART1_CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
}

void uart1_write_byte(uint8_t byte)
{
    while ((USART1_SR & USART_SR_TXE) == 0U)
    {
    }

    USART1_DR = byte;
}

void uart1_write_string(const char *text)
{
    if (text == (const char *)0)
    {
        return;
    }

    while (*text != '\0')
    {
        uart1_write_byte((uint8_t)*text);
        ++text;
    }
}

void uart1_write_hex32(uint32_t value)
{
    static const char digits[] = "0123456789ABCDEF";

    uart1_write_string("0x");

    for (uint32_t shift = 28U;; shift -= 4U)
    {
        const uint32_t nibble = (value >> shift) & 0xFUL;
        uart1_write_byte((uint8_t)digits[nibble]);

        if (shift == 0U)
        {
            break;
        }
    }
}

void uart1_write_u32(uint32_t value)
{
    static const uint32_t powers[] =
    {
        1000000000UL,
        100000000UL,
        10000000UL,
        1000000UL,
        100000UL,
        10000UL,
        1000UL,
        100UL,
        10UL,
        1UL
    };
    bool started = false;

    for (uint32_t index = 0U; index < 10U; ++index)
    {
        uint8_t digit = 0U;
        while (value >= powers[index])
        {
            value -= powers[index];
            ++digit;
        }

        if ((digit != 0U) || started || (index == 9U))
        {
            uart1_write_byte((uint8_t)('0' + digit));
            started = true;
        }
    }
}

bool uart1_byte_available(void)
{
    return (USART1_SR & USART_SR_RXNE) != 0U;
}

uint8_t uart1_read_byte(void)
{
    return (uint8_t)(USART1_DR & 0xFFUL);
}
