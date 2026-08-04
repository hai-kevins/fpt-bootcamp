#ifndef RING_QUEUE_H
#define RING_QUEUE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct
{
    uint8_t *storage;
    size_t capacity,
    item_size,
    head,
    tail,
    count;
} ring_queue_t;

bool ring_queue_init(ring_queue_t *, void *, size_t, size_t);
bool ring_queue_send(ring_queue_t *, const void *);
bool ring_queue_receive(ring_queue_t *, void *);
bool ring_queue_validate(const ring_queue_t *);

#endif
