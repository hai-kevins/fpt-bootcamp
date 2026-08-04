#include "dispatcher.h"
#include "platform.h"
#include "stm32f1.h"

#define RCC_APB2ENR_AFIOEN  (1UL << 0U)
#define RCC_APB2ENR_IOPAEN  (1UL << 2U)
#define RCC_APB2ENR_IOPCEN  (1UL << 4U)

#define LED_PIN             (13U)
#define LED_CONFIG_SHIFT    ((LED_PIN - 8U) * 4U)

static void led_gpio_init(void)
{
    RCC_APB2ENR |= RCC_APB2ENR_IOPCEN;

    GPIOC_CRH = (GPIOC_CRH & ~(0xFUL << LED_CONFIG_SHIFT)) | (0x2UL << LED_CONFIG_SHIFT);

    platform_led_off();
}

static void button_gpio_init(void)
{
    RCC_APB2ENR |= RCC_APB2ENR_AFIOEN | RCC_APB2ENR_IOPAEN;

    GPIOA_CRL = (GPIOA_CRL & ~0xFUL) | 0x8UL;

    GPIOA_ODR |= 1UL;

    AFIO_EXTICR1 &= ~ 0xFUL;

    EXTI_IMR |= 1UL;
    EXTI_RTSR |= 1UL;
    EXTI_FTSR |= 1UL;
    EXTI_PR = 1UL;

    NVIC_ISER0 = (1UL << 6U);
}

void platform_gpio_init(void)
{
    led_gpio_init();
    button_gpio_init();
}

void platform_led_on(void)
{
    GPIOC_BSRR = (1UL << (LED_PIN + 16U));
}

void platform_led_off(void)
{
    GPIOC_BSRR = (1UL << LED_PIN);
}

void platform_led_toggle(void)
{
    if (platform_led_is_on())
    {
        platform_led_off();
    }
    else
    {
        platform_led_on();
    }
}

bool platform_led_is_on(void)
{
    return (GPIOC_ODR & (1UL << LED_PIN)) == 0UL;
}

bool platform_button_is_pressed(void)
{
    return (GPIOA_IDR & 1UL) == 0UL;
}

void EXTI0_IRQHandler(void)
{
    if ((EXTI_PR & 1UL) != 0UL)
    {
        EXTI_PR = 1UL;

        (void) dispatcher_post(EVENT_SOURCE_ISR, EVENT_DESTINATION_BUTTON, SIGNAL_BUTTON_EDGE, 0UL, platform_time_now_ms());
    }
}
