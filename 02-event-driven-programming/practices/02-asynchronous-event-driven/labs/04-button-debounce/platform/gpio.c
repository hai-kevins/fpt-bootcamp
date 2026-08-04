#include "platform.h"
#include "stm32f1.h"

#define LED_PIN (13U)

void platform_systick_init(void);

void platform_init(void)
{
    RCC_APB2ENR |= (1UL << 0U) | (1UL << 2U) | (1UL << 4U);

    GPIOC_CRH = (GPIOC_CRH & ~(0xFUL << 20U)) | (0x2UL << 20U);
    platform_led_off();

    GPIOA_CRL = (GPIOA_CRL & ~0xFUL) | 0x8UL;
    GPIOA_ODR |= 1UL;

    AFIO_EXTICR1 &= ~ 0xFUL;
    EXTI_IMR |= 1UL;
    EXTI_RTSR |= 1UL;
    EXTI_FTSR |= 1UL;
    EXTI_PR = 1UL;
    NVIC_ISER0 = (1UL << 6U);

    platform_systick_init();
}

void platform_idle(void)
{
    __asm volatile("wfi" : : : "memory");
}

bool platform_button_is_pressed(void)
{
    return (GPIOA_IDR & 1UL) == 0UL;
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
    if ((GPIOC_ODR & (1UL << LED_PIN)) == 0UL)
    {
        platform_led_off();
    }
        else
        {
            platform_led_on();
        }
}

void EXTI0_IRQHandler(void)
{
    if ((EXTI_PR & 1UL) != 0UL)
    {
        const event_t event =
        {
            .timestamp_ms = platform_time_now_ms(),
            .signal = (uint16_t) EVENT_BUTTON_EDGE
        };
        EXTI_PR = 1UL;
        (void) lab04_post_from_isr(&event);
    }
}
