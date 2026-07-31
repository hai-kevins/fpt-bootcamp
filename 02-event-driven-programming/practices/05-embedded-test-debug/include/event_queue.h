#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include "event.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define EVENT_QUEUE_MAX_CAPACITY (32U)

typedef struct
{
    event_t items[EVENT_QUEUE_MAX_CAPACITY];
    size_t capacity;
    size_t head;
    size_t tail;
    size_t count;

    size_t high_water_mark;
    uint32_t post_count;
    uint32_t get_count;
    uint32_t overflow_count;
} event_queue_t;

bool event_queue_init(
    event_queue_t *queue,
    size_t capacity
);

bool event_queue_post(
    event_queue_t *queue,
    const event_t *event
);

bool event_queue_get(
    event_queue_t *queue,
    event_t *event
);

bool event_queue_is_empty(
    const event_queue_t *queue
);

#endif
