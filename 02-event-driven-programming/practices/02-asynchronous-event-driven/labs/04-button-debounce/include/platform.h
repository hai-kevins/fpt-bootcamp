#ifndef LAB04_PLATFORM_H
#define LAB04_PLATFORM_H
#include "event.h"
#include <stdbool.h>
#include <stdint.h>
void platform_init(void);
void platform_idle(void);
uint32_t platform_time_now_ms(void);
uint32_t platform_critical_enter(void);
void platform_critical_exit(uint32_t state);
bool platform_button_is_pressed(void);
void platform_led_on(void);
void platform_led_off(void);
void platform_led_toggle(void);
bool lab04_post_from_isr(const event_t *event);
#endif
