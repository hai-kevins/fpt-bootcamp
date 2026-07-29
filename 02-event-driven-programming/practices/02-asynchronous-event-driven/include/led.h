#ifndef LED_H
#define LED_H

#include "event.h"

#include <stdint.h>

typedef enum
{
    LED_STATE_OFF = 0,
    LED_STATE_ON,
    LED_STATE_BLINKING
} led_state_t;

void led_init(void);
void led_event_handler(const event_t *event);
led_state_t led_get_state(void);
uint32_t led_get_blink_period_ms(void);

#endif
