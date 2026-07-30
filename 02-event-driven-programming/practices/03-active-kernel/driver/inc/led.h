#ifndef DRIVER_LED_H
#define DRIVER_LED_H

#include <stdbool.h>

void led_init(void);
void led_on(void);
void led_off(void);
void led_toggle(void);
bool led_is_on(void);

#endif
