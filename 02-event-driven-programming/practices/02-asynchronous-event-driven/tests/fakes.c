#include "platform.h"
#include "test.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define FAKE_UART_CAPACITY (4096U)

static bool g_button_pressed;
static bool g_led_on;
static uint32_t g_time_ms;
static char g_uart_output[FAKE_UART_CAPACITY];
static size_t g_uart_length;

void fake_set_button(bool pressed)
{
    g_button_pressed = pressed;
}

bool fake_get_led(void)
{
    return g_led_on;
}

void fake_clear_uart(void)
{
    g_uart_length = 0U;
    g_uart_output[0] = '\0';
}

const char *fake_get_uart(void)
{
    return g_uart_output;
}

void platform_init(void)
{
}

void platform_idle(void)
{
}

uint32_t platform_critical_enter(void)
{
    return 0UL;
}

void platform_critical_exit(uint32_t previous_state)
{
    (void) previous_state;
}

uint32_t platform_time_now_ms(void)
{
    return g_time_ms;
}

void platform_led_on(void)
{
    g_led_on = true;
}

void platform_led_off(void)
{
    g_led_on = false;
}

void platform_led_toggle(void)
{
    g_led_on = !g_led_on;
}

bool platform_led_is_on(void)
{
    return g_led_on;
}

bool platform_button_is_pressed(void)
{
    return g_button_pressed;
}

void platform_uart_write_byte(uint8_t byte)
{
    if (g_uart_length < (FAKE_UART_CAPACITY - 1U))
    {
        g_uart_output[g_uart_length] = (char) byte;
        g_uart_length++;
        g_uart_output[g_uart_length] = '\0';
    }
}

void platform_uart_write_string(const char *text)
{
    if (text == NULL)
    {
        return;
    }

    while (*text != '\0')
    {
        platform_uart_write_byte((uint8_t)*text);
        text++;
    }
}

void fake_platform_reset(void)
{
    g_button_pressed = false;
    g_led_on = false;
    g_time_ms = 0UL;
    fake_clear_uart();
}
