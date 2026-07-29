#include "button.h"

#include "dispatcher.h"
#include "platform.h"
#include "software_timer.h"

#include <stddef.h>

#define BUTTON_DEBOUNCE_MS (20UL)

static bool g_stable_pressed;

void button_init(void)
{
    g_stable_pressed = platform_button_is_pressed();
}

void button_event_handler(const event_t *event)
{
    bool current_pressed;

    if (event == NULL)
    {
        return;
    }

    switch ((signal_t)event->signal)
    {
        case SIGNAL_BUTTON_EDGE:
        {
            (void)software_timer_start(
                SOFTWARE_TIMER_BUTTON_DEBOUNCE,
                EVENT_DESTINATION_BUTTON,
                SIGNAL_BUTTON_DEBOUNCE_TIMEOUT,
                0UL,
                BUTTON_DEBOUNCE_MS,
                false,
                event->timestamp_ms
            );
            break;
        }

        case SIGNAL_BUTTON_DEBOUNCE_TIMEOUT:
        {
            current_pressed = platform_button_is_pressed();

            if (current_pressed != g_stable_pressed)
            {
                g_stable_pressed = current_pressed;

                (void)dispatcher_post(
                    EVENT_SOURCE_BUTTON,
                    EVENT_DESTINATION_APP,
                    current_pressed
                        ? SIGNAL_BUTTON_PRESSED
                        : SIGNAL_BUTTON_RELEASED,
                    0UL,
                    event->timestamp_ms
                );
            }
            break;
        }

        default:
        {
            break;
        }
    }
}

bool button_is_stably_pressed(void)
{
    return g_stable_pressed;
}
