#ifndef EVENT_TRACE_H
#define EVENT_TRACE_H

#include "event.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifndef EVENT_TRACE_CAPACITY
#define EVENT_TRACE_CAPACITY (128U)
#endif

typedef enum
{
    EVENT_TRACE_POST = 0,
    EVENT_TRACE_DISPATCH,
    EVENT_TRACE_COMPLETE,
    EVENT_TRACE_DROP
} event_trace_phase_t;

typedef struct
{
    event_t event;
    uint8_t phase;
    uint8_t state;
    uint16_t reserved;
} event_trace_record_t;

void event_trace_init(void);
void event_trace_clear(void);
void event_trace_record(event_trace_phase_t phase, const event_t *event, uint8_t state);
size_t event_trace_count(void);
bool event_trace_get(size_t index, event_trace_record_t *record);
uint32_t event_trace_overwritten(void);

#endif
