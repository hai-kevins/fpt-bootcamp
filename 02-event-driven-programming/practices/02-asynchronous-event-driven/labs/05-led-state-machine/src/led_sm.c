#include "led_sm.h"
#include <stddef.h>
void led_sm_init(led_sm_t *sm)
{
    if (sm == NULL) return;
    sm->state = LED_OFF;
    sm->output_on = false;
    sm->period_ms = 500UL;
}
void led_sm_dispatch(
    led_sm_t *sm,
    led_event_t event,
    uint32_t parameter
)
{
    if (sm == NULL) return;
    switch (event)
    {
        case LED_EVENT_ON:
            sm->state = LED_ON;
            sm->output_on = true;
            break;
        case LED_EVENT_OFF:
        case LED_EVENT_BLINK_STOP:
            sm->state = LED_OFF;
            sm->output_on = false;
            break;
        case LED_EVENT_BLINK_START:
            sm->state = LED_BLINKING;
            sm->output_on = true;
            sm->period_ms =
                (parameter == 0UL) ? 500UL : parameter;
            break;
        case LED_EVENT_TIMEOUT:
            if (sm->state == LED_BLINKING)
                sm->output_on = !sm->output_on;
            break;
        default:
            break;
    }
}
