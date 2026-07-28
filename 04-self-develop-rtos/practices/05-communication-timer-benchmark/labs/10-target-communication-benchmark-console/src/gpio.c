#include "gpio.h"
#include "stm32f1.h"

#define LED_PIN (13UL)
#define BUTTON_PIN (0UL)

void gpio_led_init(void)
{
    RCC_APB2ENR |= RCC_APB2ENR_IOPCEN;

    GPIOC_CRH &= ~(0xFUL << 20);
    GPIOC_CRH |= (0x2UL << 20); /* PC13 output 2 MHz, push-pull. */
    gpio_led_off();
}

void gpio_led_on(void)
{
    GPIOC_BRR = (1UL << LED_PIN);
}

void gpio_led_off(void)
{
    GPIOC_BSRR = (1UL << LED_PIN);
}

void gpio_led_toggle(void)
{
    if ((GPIOC_ODR & (1UL << LED_PIN)) != 0U)
    {
        gpio_led_on();
    }
    else
    {
        gpio_led_off();
    }
}

void gpio_button_exti0_init(void)
{
    RCC_APB2ENR |= RCC_APB2ENR_AFIOEN | RCC_APB2ENR_IOPAEN;

    GPIOA_CRL &= ~(0xFUL << 0);
    GPIOA_CRL |= (0x8UL << 0); /* Input with pull-up/pull-down. */
    GPIOA_ODR &= ~(1UL << BUTTON_PIN); /* Pull-down. */

    AFIO_EXTICR1 &= ~0xFUL; /* EXTI0 from PA0. */
    EXTI_IMR |= (1UL << BUTTON_PIN);
    EXTI_RTSR |= (1UL << BUTTON_PIN);
    EXTI_FTSR &= ~(1UL << BUTTON_PIN);
    EXTI_PR = (1UL << BUTTON_PIN);

    NVIC_IPR0 = 0x60U;
    NVIC_ISER0 = (1UL << 6); /* EXTI0 IRQ number 6. */
}

bool gpio_button_is_high(void)
{
    return (GPIOA_IDR & (1UL << BUTTON_PIN)) != 0U;
}
