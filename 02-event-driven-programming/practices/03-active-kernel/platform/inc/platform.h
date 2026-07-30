#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdbool.h>
#include <stdint.h>

void platform_init(void);
void platform_idle(void);

void platform_led_init(void);
void platform_led_on(void);
void platform_led_off(void);
void platform_led_toggle(void);
bool platform_led_is_on(void);

void platform_button_init(void);
bool platform_button_is_pressed(void);

void platform_uart_init(void);
void platform_uart_write_byte(uint8_t byte);
void platform_uart_write_string(const char *text);

void platform_systick_init(void);
uint32_t platform_time_now_ms(void);

#endif
