#include "event_trace.h"

static event_trace_record_t g_records[EVENT_TRACE_CAPACITY];
static size_t g_head;
static size_t g_count;
static uint32_t g_overwritten;

void event_trace_init(void)
{
    g_head = 0U;
    g_count = 0U;
    g_overwritten = 0UL;
}

void event_trace_clear(void)
{
    event_trace_init();
}

void event_trace_record(
    event_trace_phase_t phase,
    const event_t *event,
    uint8_t state
)
{
    event_trace_record_t *record;

    if (event == NULL)
    {
        return;
    }

    record = &g_records[g_head];
    record->event = *event;
    record->phase = (uint8_t)phase;
    record->state = state;
    record->reserved = 0U;

    g_head = (g_head + 1U) % EVENT_TRACE_CAPACITY;

    if (g_count < EVENT_TRACE_CAPACITY)
    {
        g_count++;
    }
    else
    {
        g_overwritten++;
    }
}

size_t event_trace_count(void)
{
    return g_count;
}

bool event_trace_get(size_t index, event_trace_record_t *record)
{
    size_t oldest;
    size_t physical_index;

    if ((record == NULL) || (index >= g_count))
    {
        return false;
    }

    oldest =
        (g_head + EVENT_TRACE_CAPACITY - g_count) %
        EVENT_TRACE_CAPACITY;
    physical_index = (oldest + index) % EVENT_TRACE_CAPACITY;

    *record = g_records[physical_index];
    return true;
}

uint32_t event_trace_overwritten(void)
{
    return g_overwritten;
}
