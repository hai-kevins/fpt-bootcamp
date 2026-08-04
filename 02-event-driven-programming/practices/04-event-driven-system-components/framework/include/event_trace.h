#ifndef ED_EVENT_TRACE_H
#define ED_EVENT_TRACE_H

#include "event.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define ED_TRACE_CAPACITY (128U)

typedef enum
{
    ED_TRACE_POST = 0,
    ED_TRACE_ENQUEUE,
    ED_TRACE_DISPATCH,
    ED_TRACE_TRANSITION,
    ED_TRACE_RELEASE,
    ED_TRACE_DROP,
    ED_TRACE_LINK_TX,
    ED_TRACE_LINK_RX
} ed_trace_type_t;

typedef struct
{
    uint32_t timestamp;
    ed_signal_t signal;
    uint8_t source;
    uint8_t destination;
    uint8_t type;
    uint8_t state;
    uint16_t argument;
} ed_trace_record_t;

typedef struct
{
    ed_trace_record_t records[ED_TRACE_CAPACITY];
    size_t head;
    size_t count;
    uint32_t overwritten_count;
} ed_trace_t;

void ed_trace_init(ed_trace_t *trace);
void ed_trace_write(ed_trace_t *trace, const ed_trace_record_t *record);
bool ed_trace_get_oldest(const ed_trace_t *trace, size_t index, ed_trace_record_t *record);

#endif
