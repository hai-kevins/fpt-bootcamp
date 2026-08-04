#ifndef LAB04_EVENT_H
#define LAB04_EVENT_H

#include <stdint.h>

typedef enum
{
    EVENT_NONE = 0,
    EVENT_BUTTON_EDGE,
    EVENT_DEBOUNCE_TIMEOUT
} event_signal_t;

typedef struct
{
    uint32_t timestamp_ms;
    uint16_t signal;
} event_t;

#endif
