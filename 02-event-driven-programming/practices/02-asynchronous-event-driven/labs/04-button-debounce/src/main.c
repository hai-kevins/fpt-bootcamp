#include "event_queue.h"
#include "platform.h"
#include "software_timer.h"

static event_queue_t g_queue;
static bool g_stable_pressed;

static bool post_event(const event_t *event)
{
    uint32_t key;
    bool result;
    key = platform_critical_enter();
    result = event_queue_post(&g_queue, event);
    platform_critical_exit(key);
    return result;
}

bool lab04_post_from_isr(const event_t *event)
{
    return post_event(event);
}

int main(void)
{
    event_t event;

    event_queue_init(&g_queue);
    software_timer_init();
    platform_init();

    g_stable_pressed = platform_button_is_pressed();

    if (g_stable_pressed)
    {
        platform_led_on();
    }
    else
    {
        platform_led_off();
    }

    for (;;)
    {
        if (software_timer_expired(platform_time_now_ms()))
        {
            const event_t timeout =
            {
                .timestamp_ms = platform_time_now_ms(),
                .signal = (uint16_t)EVENT_DEBOUNCE_TIMEOUT
            };
            (void)post_event(&timeout);
        }

        if (!event_queue_get(&g_queue, &event))
        {
            platform_idle();
            continue;
        }

        if ((event_signal_t)event.signal == EVENT_BUTTON_EDGE)
        {
            software_timer_start(event.timestamp_ms, 20UL);
        }
        else if ((event_signal_t)event.signal ==
                 EVENT_DEBOUNCE_TIMEOUT)
        {
            const bool current =
                platform_button_is_pressed();

            if (current != g_stable_pressed)
            {
                g_stable_pressed = current;

                if (current)
                {
                    platform_led_on();
                }
                else
                {
                    platform_led_off();
                }
            }
        }
    }
}
