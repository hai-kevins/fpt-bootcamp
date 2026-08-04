#ifndef LAB05_LED_SM_H
#define LAB05_LED_SM_H

#include <stdbool.h>
#include <stdint.h>

typedef enum 
{ 
    LED_OFF=0, 
    LED_ON, 
    LED_BLINKING 
} led_state_t;

typedef enum
{
    LED_EVENT_ON=0,
    LED_EVENT_OFF,
    LED_EVENT_BLINK_START,
    LED_EVENT_BLINK_STOP,
    LED_EVENT_TIMEOUT
} led_event_t;

typedef struct
{
    led_state_t state;
    bool output_on;
    uint32_t period_ms;
} led_sm_t;

void led_sm_init(led_sm_t *sm);
void led_sm_dispatch(
    led_sm_t *sm,
    led_event_t event,
    uint32_t parameter
);

#endif
