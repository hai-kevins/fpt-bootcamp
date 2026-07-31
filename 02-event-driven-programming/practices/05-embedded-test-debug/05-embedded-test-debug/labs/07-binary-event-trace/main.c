#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define CAPACITY (128U)

typedef struct
{
    uint32_t time;
    uint16_t signal;
} record_t;

typedef struct
{
    record_t records[CAPACITY];
    size_t head;
    size_t count;
    uint32_t overwritten;
} trace_t;

static void write_record(trace_t *trace, record_t record)
{
    trace->records[trace->head] = record;
    trace->head = (trace->head + 1U) % CAPACITY;

    if (trace->count < CAPACITY)
    {
        trace->count++;
    }
    else
    {
        trace->overwritten++;
    }
}

static void clear(trace_t *trace)
{
    trace->head = 0U;
    trace->count = 0U;
    trace->overwritten = 0U;
}

int main(void)
{
    trace_t trace = {0};

    for (uint32_t i = 0U; i < CAPACITY + 5U; i++)
    {
        const record_t record = {.time = i, .signal = (uint16_t)i};
        write_record(&trace, record);
    }

    const size_t oldest_index =
        (trace.head + CAPACITY - trace.count) % CAPACITY;
    const uint32_t oldest = trace.records[oldest_index].time;

    const bool before_clear =
        (trace.count == CAPACITY) &&
        (trace.overwritten == 5U) &&
        (oldest == 5U);

    clear(&trace);
    const bool clear_pass =
        (trace.count == 0U) &&
        (trace.overwritten == 0U);

    (void)printf(
        "count=128 overwritten=5 oldest=5 clear=%s\n",
        clear_pass ? "PASS" : "FAIL"
    );

    return (before_clear && clear_pass) ? 0 : 1;
}
