#include "dispatcher.h"
#include "platform.h"
#include "stm32f1.h"

#define RCC_APB2ENR_AFIOEN    (1UL << 0U)
#define RCC_APB2ENR_IOPAEN    (1UL << 2U)
#define RCC_APB2ENR_USART1EN  (1UL << 14U)

#define USART_SR_RXNE         (1UL << 5U)
#define USART_SR_TXE          (1UL << 7U)

#define USART_CR1_RE          (1UL << 2U)
#define USART_CR1_TE          (1UL << 3U)
#define USART_CR1_RXNEIE      (1UL << 5U)
#define USART_CR1_UE          (1UL << 13U)

static void uart_init(void)
{
    RCC_APB2ENR |=
        RCC_APB2ENR_AFIOEN |
        RCC_APB2ENR_IOPAEN |
        RCC_APB2ENR_USART1EN;

    GPIOA_CRH =
        (GPIOA_CRH &
         ~((0xFUL << 4U) | (0xFUL << 8U))) |
        (0xAUL << 4U) |
        (0x4UL << 8U);

    USART1_CR1 = 0UL;
    USART1_CR2 = 0UL;
    USART1_CR3 = 0UL;

    /*
     * PCLK2 = HSI 8 MHz, baud = 9600:
     * USARTDIV register value ~= 8,000,000 / 9,600 = 833 = 0x0341.
     */
    USART1_BRR = 0x0341UL;

    USART1_CR1 =
        USART_CR1_UE |
        USART_CR1_TE |
        USART_CR1_RE |
        USART_CR1_RXNEIE;

    NVIC_ISER1 = (1UL << 5U);
}

void platform_uart_write_byte(uint8_t byte)
{
    while ((USART1_SR & USART_SR_TXE) == 0UL)
    {
    }

    USART1_DR = (uint32_t)byte;
}

void platform_uart_write_string(const char *text)
{
    if (text == 0)
    {
        return;
    }

    while (*text != '\0')
    {
        platform_uart_write_byte((uint8_t)*text);
        text++;
    }
}

void USART1_IRQHandler(void)
{
    if ((USART1_SR & USART_SR_RXNE) != 0UL)
    {
        const uint8_t byte =
            (uint8_t)(USART1_DR & 0xFFUL);

        (void)dispatcher_post(
            EVENT_SOURCE_ISR,
            EVENT_DESTINATION_UART,
            SIGNAL_UART_RX_BYTE,
            (uint32_t)byte,
            platform_time_now_ms()
        );
    }
}

void platform_uart_start(void)
{
    uart_init();
}
