#ifndef ED_EVENT_H
#define ED_EVENT_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define ED_EVENT_PAYLOAD_MAX (16U)
#define ED_EVENT_FLAG_DYNAMIC (1U << 0U)
#define ED_EVENT_FLAG_REMOTE  (1U << 1U)

typedef uint16_t ed_signal_t;

typedef struct
{
    ed_signal_t signal;
    uint8_t source;
    uint8_t destination;
    uint8_t flags;
    uint8_t ref_count;
    uint8_t payload_length;
    uint8_t pool_index;
    uint16_t sequence;
    uint8_t payload[ED_EVENT_PAYLOAD_MAX];
} ed_event_t;

void ed_event_init_static(ed_event_t *event, ed_signal_t signal, uint8_t source, uint8_t destination);

bool ed_event_set_payload(ed_event_t *event, const void *payload, size_t length);

#endif
