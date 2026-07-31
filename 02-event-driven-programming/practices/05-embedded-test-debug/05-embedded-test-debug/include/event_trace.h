#ifndef EVENT_TRACE_H
#define EVENT_TRACE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define EVENT_TRACE_CAPACITY (32U)

typedef enum
{
    TRACE_TYPE_POST = 1,
    TRACE_TYPE_DISPATCH,
    TRACE_TYPE_STATE,
    TRACE_TYPE_TIMER,
    TRACE_TYPE_FAULT
} trace_type_t;

typedef struct
{
    uint32_t timestamp_ms;
    uint16_t signal;
    uint16_t argument;
    uint8_t type;
    uint8_t source;
    uint8_t destination;
    uint8_t state;
} event_trace_record_t;

typedef struct
{
    event_trace_record_t records[EVENT_TRACE_CAPACITY];
    size_t head;
    size_t count;
    uint32_t overwritten_count;
} event_trace_t;

void event_trace_init(event_trace_t *trace);
void event_trace_write(
    event_trace_t *trace,
    const event_trace_record_t *record
);
bool event_trace_get_oldest(
    const event_trace_t *trace,
    size_t index,
    event_trace_record_t *record
);

#endif
