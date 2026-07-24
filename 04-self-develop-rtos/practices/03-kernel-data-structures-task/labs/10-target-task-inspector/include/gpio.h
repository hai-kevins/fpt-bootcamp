#ifndef GPIO_H
#define GPIO_H

#include <stdbool.h>

void gpio_led_init(void);
void gpio_led_on(void);
void gpio_led_off(void);
void gpio_led_toggle(void);
void gpio_button_exti0_init(void);
bool gpio_button_is_high(void);

#endif
