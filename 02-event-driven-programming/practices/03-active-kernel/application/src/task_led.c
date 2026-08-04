#include "task_led.h"

#include "app_signals.h"
#include "app_tasks.h"
#include "led.h"
#include "timer.h"

#include "runtime.h"

typedef enum
{
    LED_STATE_OFF = 0,
    LED_STATE_ON,
    LED_STATE_BLINKING
} led_state_t;

static led_state_t g_state;
static ak_timer_id_t g_blink_timer;
static uint32_t g_period_ms;

void task_led_init(void)
{
    g_state = LED_STATE_OFF;
    g_blink_timer = AK_TIMER_ID_INVALID;
    g_period_ms = 500U;
    led_off();
}

static void stop_timer(void)
{
    if (g_blink_timer != AK_TIMER_ID_INVALID)
    {
        (void) ak_timer_cancel(g_blink_timer);
        g_blink_timer = AK_TIMER_ID_INVALID;
    }
}

void task_led_handler(const ak_message_t *message)
{
    if (message->signal == APP_SIGNAL_LED_ON)
    {
        stop_timer();
        g_state = LED_STATE_ON;
        led_on();
    }
    else if (message->signal == APP_SIGNAL_LED_OFF)
    {
        stop_timer();
        g_state = LED_STATE_OFF;
        led_off();
    }
    else if (message->signal == APP_SIGNAL_LED_TOGGLE)
    {
        led_toggle();
        g_state = led_is_on() ? LED_STATE_ON : LED_STATE_OFF;
    }
    else if (message->signal == APP_SIGNAL_LED_BLINK_START)
    {
        uint32_t period = 500U;
        if ((message->length == sizeof(period)) && (message->kind != AK_MESSAGE_PURE))
        {
            (void) memcpy(&period, message->payload, sizeof(period));
        }
        if (period == 0U)
        {
            period = 500U;
        }
        stop_timer();
        g_period_ms = period;
        g_state = LED_STATE_BLINKING;
        g_blink_timer = ak_timer_start_periodic(APP_TASK_LED, APP_TASK_LED, APP_SIGNAL_LED_BLINK_TICK, g_period_ms);
    }
    else if (message->signal == APP_SIGNAL_LED_BLINK_STOP)
    {
        stop_timer();
        g_state = LED_STATE_OFF;
        led_off();
    }
    else if ((message->signal == APP_SIGNAL_LED_BLINK_TICK) && (g_state == LED_STATE_BLINKING))
    {
        led_toggle();
    }
}
