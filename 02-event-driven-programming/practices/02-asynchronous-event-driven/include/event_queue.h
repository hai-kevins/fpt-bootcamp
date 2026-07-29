#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include "event.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifndef EVENT_QUEUE_CAPACITY
#define EVENT_QUEUE_CAPACITY (32U)
#endif

typedef struct
{
    event_t buffer[EVENT_QUEUE_CAPACITY];
    uint16_t head;
    uint16_t tail;
    uint16_t count;
    uint16_t high_water_mark;
    uint32_t post_count;
    uint32_t get_count;
    uint32_t overflow_count;
} event_queue_t;

void event_queue_init(event_queue_t *queue);
bool event_queue_post(event_queue_t *queue, const event_t *event);
bool event_queue_get(event_queue_t *queue, event_t *event);
bool event_queue_is_empty(const event_queue_t *queue);
bool event_queue_is_full(const event_queue_t *queue);
uint16_t event_queue_count(const event_queue_t *queue);

#endif
