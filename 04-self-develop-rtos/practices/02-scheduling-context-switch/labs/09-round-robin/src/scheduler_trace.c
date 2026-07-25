#include "scheduler_trace.h"
#include "critical_section.h"

#define RTOS_TRACE_CAPACITY (64U)

static rtos_trace_record_t g_trace[RTOS_TRACE_CAPACITY];
static size_t g_trace_write_index;
static size_t g_trace_count;
static uint32_t g_trace_sequence;

void rtos_trace_reset(void)
{
    const rtos_irq_state_t state = rtos_critical_enter();
    g_trace_write_index = 0U;
    g_trace_count = 0U;
    g_trace_sequence = 0U;
    rtos_critical_exit(state);
}

void rtos_trace_record(rtos_trace_type_t type,
                     uint8_t from_task,
                     uint8_t to_task,
                     uint32_t tick)
{
    rtos_trace_record_t *record = &g_trace[g_trace_write_index];

    record->tick = tick;
    record->sequence = g_trace_sequence;
    record->type = type;
    record->from_task = from_task;
    record->to_task = to_task;

    ++g_trace_sequence;
    g_trace_write_index = (g_trace_write_index + 1U) % RTOS_TRACE_CAPACITY;
    if (g_trace_count < RTOS_TRACE_CAPACITY)
    {
        ++g_trace_count;
    }
}

size_t rtos_trace_snapshot(rtos_trace_record_t *output, size_t capacity)
{
    size_t count;
    size_t first;
    const rtos_irq_state_t state = rtos_critical_enter();

    if ((output == (rtos_trace_record_t *)0) || (capacity == 0U))
    {
        rtos_critical_exit(state);
        return 0U;
    }

    count = (g_trace_count < capacity) ? g_trace_count : capacity;
    first = (g_trace_write_index + RTOS_TRACE_CAPACITY - g_trace_count) %
            RTOS_TRACE_CAPACITY;

    for (size_t index = 0U; index < count; ++index)
    {
        output[index] = g_trace[(first + index) % RTOS_TRACE_CAPACITY];
    }

    rtos_critical_exit(state);
    return count;
}
