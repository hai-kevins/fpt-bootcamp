#include "led.h"

#include "platform.h"
#include "software_timer.h"

#include <stddef.h>

#define LED_DEFAULT_BLINK_MS (500UL)

static led_state_t g_state;
static uint32_t g_blink_period_ms;

void led_init(void)
{
    g_state = LED_STATE_OFF;
    g_blink_period_ms = LED_DEFAULT_BLINK_MS;
    platform_led_off();
}

void led_event_handler(const event_t *event)
{
    uint32_t period_ms;

    if (event == NULL)
    {
        return;
    }

    switch ((signal_t) event->signal)
    {
        case SIGNAL_LED_ON :
        {
            (void) software_timer_cancel(SOFTWARE_TIMER_LED_BLINK);
            platform_led_on();
            g_state = LED_STATE_ON;
            break;
        }

        case SIGNAL_LED_OFF :
        {
            (void) software_timer_cancel(SOFTWARE_TIMER_LED_BLINK);
            platform_led_off();
            g_state = LED_STATE_OFF;
            break;
        }

        case SIGNAL_LED_BLINK_START :
        {
            period_ms = event->parameter;

            if (period_ms == 0UL)
            {
                period_ms = LED_DEFAULT_BLINK_MS;
            }

            g_blink_period_ms = period_ms;
            platform_led_on();
            g_state = LED_STATE_BLINKING;

            (void) software_timer_start(SOFTWARE_TIMER_LED_BLINK, EVENT_DESTINATION_LED, SIGNAL_LED_TIMEOUT, 0UL,
                period_ms, true, event->timestamp_ms);
            break;
        }

        case SIGNAL_LED_BLINK_STOP :
        {
            (void) software_timer_cancel(SOFTWARE_TIMER_LED_BLINK);
            platform_led_off();
            g_state = LED_STATE_OFF;
            break;
        }

        case SIGNAL_LED_TIMEOUT :
        {
            if (g_state == LED_STATE_BLINKING)
            {
                platform_led_toggle();
            }
            break;
        }

        default:
        {
            break;
        }
    }
}

led_state_t led_get_state(void)
{
    return g_state;
}

uint32_t led_get_blink_period_ms(void)
{
    return g_blink_period_ms;
}
