#include "uart.h"
#include "gpio.h"
#include "stm32f1.h"

void uart_init(uint32_t baud_rate, uint32_t peripheral_clock_hz)
{
    if ((baud_rate == 0UL) || (peripheral_clock_hz == 0UL))
    {
        return;
    }

    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN | RCC_APB2ENR_IOPAEN | RCC_APB2ENR_USART1EN;

    gpio_config_alternate_push_pull(GPIO_PORT_A, 9UL);
    gpio_config_input_floating(GPIO_PORT_A, 10UL);

    USART1->CR1 = 0UL;
    USART1->CR2 = 0UL;
    USART1->CR3 = 0UL;
    USART1->BRR = (peripheral_clock_hz + (baud_rate / 2UL)) / baud_rate;
    USART1->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
}

void uart_write_char(char value)
{
    while ((USART1->SR & USART_SR_TXE) == 0UL)
    {
    }

    USART1->DR = (uint32_t)(uint8_t) value;
}

void uart_write_string(const char *text)
{
    if (text == (const char *)0)
    {
        return;
    }

    while (*text != '\0')
    {
        uart_write_char (*text);
        text++;
    }
}

void uart_write_line(const char *text)
{
    uart_write_string(text);
    uart_write_string("\r\n");
}

void uart_write_u32(uint32_t value)
{
    char digits[10];
    size_t count = 0U;

    if (value == 0UL)
    {
        uart_write_char('0');
        return;
    }

    while ((value > 0UL) && (count < sizeof(digits)))
    {
        digits[count] = (char)('0' + (value % 10UL));
        value /= 10UL;
        count++;
    }

    while (count > 0U)
    {
        count--;
        uart_write_char(digits[count]);
    }
}

void uart_write_size(size_t value)
{
    uart_write_u32((uint32_t) value);
}

void uart_write_hex32(uint32_t value)
{
    static const char hex[] = "0123456789ABCDEF";
    uint32_t shift;

    uart_write_string("0x");

    for (shift = 28UL;; shift -= 4UL)
    {
        uart_write_char(hex[(value >> shift) & 0x0FUL]);
        if (shift == 0UL)
        {
            break;
        }
    }
}

void uart_write_pointer(const void *pointer)
{
    uart_write_hex32((uint32_t)(uintptr_t) pointer);
}

bool uart_try_read_char(char *value)
{
    if ((value == (char *)0) || ((USART1->SR & USART_SR_RXNE) == 0UL))
    {
        return false;
    }

    *value = (char)(uint8_t) USART1->DR;
    return true;
}
