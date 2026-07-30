#include "event_trace.h"

#include <string.h>

void ed_trace_init(ed_trace_t *trace)
{
    if (trace != NULL)
    {
        (void)memset(trace, 0, sizeof(*trace));
    }
}

void ed_trace_write(ed_trace_t *trace,
                    const ed_trace_record_t *record)
{
    if ((trace == NULL) || (record == NULL))
    {
        return;
    }

    trace->records[trace->head] = *record;
    trace->head = (trace->head + 1U) % ED_TRACE_CAPACITY;

    if (trace->count < ED_TRACE_CAPACITY)
    {
        trace->count++;
    }
    else
    {
        trace->overwritten_count++;
    }
}

bool ed_trace_get_oldest(const ed_trace_t *trace,
                         size_t index,
                         ed_trace_record_t *record)
{
    if ((trace == NULL) || (record == NULL) ||
        (index >= trace->count))
    {
        return false;
    }

    const size_t oldest =
        (trace->head + ED_TRACE_CAPACITY - trace->count) %
        ED_TRACE_CAPACITY;
    const size_t physical = (oldest + index) % ED_TRACE_CAPACITY;
    *record = trace->records[physical];
    return true;
}
