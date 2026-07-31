#ifndef EVENT_H
#define EVENT_H

#include <stdint.h>

typedef enum
{
    EVENT_SIGNAL_NONE = 0,
    EVENT_SIGNAL_APP_START = 1,
    EVENT_SIGNAL_APP_STOP,
    EVENT_SIGNAL_APP_FAIL,
    EVENT_SIGNAL_APP_RESET,
    EVENT_SIGNAL_APP_TIMEOUT,
    EVENT_SIGNAL_DIAGNOSTIC
} event_signal_t;

typedef struct
{
    uint16_t signal;
    uint16_t source;
    uint32_t argument;
    uint32_t timestamp_ms;
} event_t;

#endif
