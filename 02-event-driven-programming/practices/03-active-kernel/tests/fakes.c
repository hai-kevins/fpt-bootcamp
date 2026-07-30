#include "ak_port.h"
#include "button.h"
#include "led.h"
#include "uart.h"

#include <stdbool.h>
#include <stdint.h>

uint32_t g_fake_time;
bool g_fake_button_pressed;
bool g_fake_led_on;
uint32_t g_fake_reset_count;

uint32_t ak_port_critical_enter(void) { return 0U; }
void ak_port_critical_exit(uint32_t previous_state) { (void)previous_state; }
uint32_t ak_port_time_now_ms(void) { return g_fake_time; }
void ak_port_reset(void) { g_fake_reset_count++; }

void led_init(void) { g_fake_led_on = false; }
void led_on(void) { g_fake_led_on = true; }
void led_off(void) { g_fake_led_on = false; }
void led_toggle(void) { g_fake_led_on = !g_fake_led_on; }
bool led_is_on(void) { return g_fake_led_on; }

void button_init(void) {}
bool button_is_pressed(void) { return g_fake_button_pressed; }

void uart_init(void) {}
void uart_write_byte(uint8_t byte) { (void)byte; }
void uart_write_string(const char *text) { (void)text; }
