#include "gpio.h"

#include <stdint.h>

#define REG32(address) (*(volatile uint32_t *)(uintptr_t)(address))

#define RCC_BASE_ADDRESS   (0x40021000UL)
#define GPIOC_BASE_ADDRESS (0x40011000UL)

#define RCC_APB2ENR REG32(RCC_BASE_ADDRESS + 0x18UL)
#define GPIOC_CRH   REG32(GPIOC_BASE_ADDRESS + 0x04UL)
#define GPIOC_ODR   REG32(GPIOC_BASE_ADDRESS + 0x0CUL)
#define GPIOC_BSRR  REG32(GPIOC_BASE_ADDRESS + 0x10UL)

#define RCC_APB2ENR_IOPCEN  (1UL << 4U)
#define LED_PIN             (13U)
#define LED_CONFIG_SHIFT    ((LED_PIN - 8U) * 4U)
#define GPIO_CONFIG_MASK    (0xFUL)
#define GPIO_OUTPUT_PP_2MHZ (0x2UL)

void gpio_led_init(void)
{
    RCC_APB2ENR |= RCC_APB2ENR_IOPCEN;

    GPIOC_CRH = (GPIOC_CRH & ~(GPIO_CONFIG_MASK << LED_CONFIG_SHIFT)) | (GPIO_OUTPUT_PP_2MHZ << LED_CONFIG_SHIFT);

    gpio_led_off();
}

void gpio_led_on(void)
{
    /* Blue Pill LED on PC13 is active-low. */ GPIOC_BSRR = (1UL << (LED_PIN + 16U));
}

void gpio_led_off(void)
{
    GPIOC_BSRR = (1UL << LED_PIN);
}

void gpio_led_write(bool enabled)
{
    if (enabled)
    {
        gpio_led_on();
    }
    else
    {
        gpio_led_off();
    }
}

void gpio_led_toggle(void)
{
    if (gpio_led_is_on())
    {
        gpio_led_off();
    }
    else
    {
        gpio_led_on();
    }
}

bool gpio_led_is_on(void)
{
    return (GPIOC_ODR & (1UL << LED_PIN)) == 0UL;
}
