#include "gpio.h"
#include "stm32f1.h"

#define GPIO_PIN_COUNT (16UL)

static stm32_gpio_t *gpio_from_port(gpio_port_t port)
{
    switch (port)
    {
        case GPIO_PORT_A : return GPIOA;
        case GPIO_PORT_B : return GPIOB;
        case GPIO_PORT_C : return GPIOC;
        default:
        return (stm32_gpio_t *)0;
    }
}

static void configure_nibble(stm32_gpio_t *gpio, uint32_t pin, uint32_t value)
{
    volatile uint32_t *reg;
    uint32_t shift;
    uint32_t current;

    if ((gpio == (stm32_gpio_t *)0) || (pin >= GPIO_PIN_COUNT))
    {
        return;
    }

    if (pin < 8UL)
    {
        reg = &gpio->CRL;
        shift = pin * 4UL;
    }
    else
    {
        reg = &gpio->CRH;
        shift = (pin - 8UL) * 4UL;
    }

    current = *reg;
    current &= ~ (0x0FUL << shift);
    current |= (value & 0x0FUL) << shift;
    *reg = current;
}

void gpio_enable_clock(gpio_port_t port)
{
    switch (port)
    {
        case GPIO_PORT_A : RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
        break;
        case GPIO_PORT_B : RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
        break;
        case GPIO_PORT_C : RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
        break;
        default:
        break;
    }
}

void gpio_config_output_push_pull(gpio_port_t port, uint32_t pin)
{
    configure_nibble(gpio_from_port(port), pin, 0x03UL);
}

void gpio_config_alternate_push_pull(gpio_port_t port, uint32_t pin)
{
    configure_nibble(gpio_from_port(port), pin, 0x0BUL);
}

void gpio_config_input_floating(gpio_port_t port, uint32_t pin)
{
    configure_nibble(gpio_from_port(port), pin, 0x04UL);
}

void gpio_write(gpio_port_t port, uint32_t pin, bool high)
{
    stm32_gpio_t *gpio = gpio_from_port(port);

    if ((gpio == (stm32_gpio_t *)0) || (pin >= GPIO_PIN_COUNT))
    {
        return;
    }

    if (high)
    {
        gpio->BSRR = 1UL << pin;
    }
    else
    {
        gpio->BRR = 1UL << pin;
    }
}

void gpio_toggle(gpio_port_t port, uint32_t pin)
{
    stm32_gpio_t *gpio = gpio_from_port(port);

    if ((gpio == (stm32_gpio_t *)0) || (pin >= GPIO_PIN_COUNT))
    {
        return;
    }

    gpio_write(port, pin, (gpio->ODR & (1UL << pin)) == 0UL);
}
