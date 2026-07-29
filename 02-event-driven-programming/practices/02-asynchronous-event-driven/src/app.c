#include "app.h"

#include "dispatcher.h"

#include <stddef.h>

static app_state_t g_state;

static void post_led(
    signal_t signal,
    uint32_t parameter,
    uint32_t timestamp_ms
)
{
    (void)dispatcher_post(
        EVENT_SOURCE_APP,
        EVENT_DESTINATION_LED,
        signal,
        parameter,
        timestamp_ms
    );
}

static void post_uart_status(
    signal_t signal,
    uint32_t timestamp_ms
)
{
    (void)dispatcher_post(
        EVENT_SOURCE_APP,
        EVENT_DESTINATION_UART,
        signal,
        0UL,
        timestamp_ms
    );
}

void app_init(void)
{
    g_state = APP_STATE_BOOT;
}

void app_event_handler(const event_t *event)
{
    if (event == NULL)
    {
        return;
    }

    switch ((signal_t)event->signal)
    {
        case SIGNAL_SYSTEM_START:
        {
            g_state = APP_STATE_IDLE;
            post_led(SIGNAL_LED_OFF, 0UL, event->timestamp_ms);
            post_uart_status(
                SIGNAL_UART_STATUS_IDLE,
                event->timestamp_ms
            );
            break;
        }

        case SIGNAL_BUTTON_PRESSED:
        {
            if (g_state == APP_STATE_IDLE)
            {
                g_state = APP_STATE_ACTIVE;
                post_led(
                    SIGNAL_LED_BLINK_START,
                    500UL,
                    event->timestamp_ms
                );
                post_uart_status(
                    SIGNAL_UART_STATUS_ACTIVE,
                    event->timestamp_ms
                );
            }
            else if (g_state == APP_STATE_ACTIVE)
            {
                g_state = APP_STATE_IDLE;
                post_led(
                    SIGNAL_LED_OFF,
                    0UL,
                    event->timestamp_ms
                );
                post_uart_status(
                    SIGNAL_UART_STATUS_IDLE,
                    event->timestamp_ms
                );
            }
            break;
        }

        case SIGNAL_ERROR_DETECTED:
        {
            g_state = APP_STATE_ERROR;
            post_led(
                SIGNAL_LED_BLINK_START,
                100UL,
                event->timestamp_ms
            );
            post_uart_status(
                SIGNAL_UART_STATUS_ERROR,
                event->timestamp_ms
            );
            break;
        }

        case SIGNAL_RESET_REQUEST:
        {
            g_state = APP_STATE_IDLE;
            post_led(SIGNAL_LED_OFF, 0UL, event->timestamp_ms);
            post_uart_status(
                SIGNAL_UART_STATUS_IDLE,
                event->timestamp_ms
            );
            break;
        }

        case SIGNAL_UART_STATUS_REQUEST:
        {
            signal_t status_signal = SIGNAL_UART_STATUS_BOOT;

            if (g_state == APP_STATE_IDLE)
            {
                status_signal = SIGNAL_UART_STATUS_IDLE;
            }
            else if (g_state == APP_STATE_ACTIVE)
            {
                status_signal = SIGNAL_UART_STATUS_ACTIVE;
            }
            else if (g_state == APP_STATE_ERROR)
            {
                status_signal = SIGNAL_UART_STATUS_ERROR;
            }

            post_uart_status(status_signal, event->timestamp_ms);
            break;
        }

        case SIGNAL_SENSOR_READY:
        case SIGNAL_BUTTON_RELEASED:
        default:
        {
            break;
        }
    }
}

app_state_t app_get_state(void)
{
    return g_state;
}

const char *app_state_name(app_state_t state)
{
    switch (state)
    {
        case APP_STATE_BOOT:
            return "BOOT";
        case APP_STATE_IDLE:
            return "IDLE";
        case APP_STATE_ACTIVE:
            return "ACTIVE";
        case APP_STATE_ERROR:
            return "ERROR";
        default:
            return "UNKNOWN";
    }
}
