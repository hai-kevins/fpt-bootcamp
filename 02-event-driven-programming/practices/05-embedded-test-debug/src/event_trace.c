#include "event_trace.h"

#include <string.h>

void event_trace_init(event_trace_t *trace)
{
    if (trace != NULL)
    {
        (void) memset(trace, 0, sizeof (*trace));
    }
}

void event_trace_write(event_trace_t *trace, const event_trace_record_t *record)
{
    if ((trace == NULL) || (record == NULL))
    {
        return;
    }

    trace->records[trace->head] = *record;
    trace->head = (trace->head + 1U) % EVENT_TRACE_CAPACITY;

    if (trace->count < EVENT_TRACE_CAPACITY)
    {
        trace->count++;
    }
    else
    {
        trace->overwritten_count++;
    }
}

bool event_trace_get_oldest(const event_trace_t *trace, size_t index, event_trace_record_t *record)
{
    if ((trace == NULL) || (record == NULL) || (index >= trace->count))
    {
        return false;
    }

    const size_t oldest = (trace->head + EVENT_TRACE_CAPACITY - trace->count) % EVENT_TRACE_CAPACITY;

    const size_t position = (oldest + index) % EVENT_TRACE_CAPACITY;

    *record = trace->records[position];
    return true;
}
