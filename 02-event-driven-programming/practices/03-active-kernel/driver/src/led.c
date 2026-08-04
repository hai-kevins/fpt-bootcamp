#include "led.h"
#include "platform.h"

void led_init(void)
{
    platform_led_init();
}

void led_on(void)
{
    platform_led_on();
}

void led_off(void)
{
    platform_led_off();
}

void led_toggle(void)
{
    platform_led_toggle();
}

bool led_is_on(void)
{
    return platform_led_is_on();
}
