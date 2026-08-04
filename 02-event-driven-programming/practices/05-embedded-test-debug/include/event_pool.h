#ifndef EVENT_POOL_H
#define EVENT_POOL_H

#include "event.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define EVENT_POOL_MAX_BLOCKS (16U)

typedef struct
{
    event_t event;
    uint16_t reference_count;
    bool used;
} event_pool_block_t;

typedef struct
{
    event_pool_block_t blocks[EVENT_POOL_MAX_BLOCKS];
    size_t capacity;
    size_t used_count;
    size_t high_water_mark;
    uint32_t allocation_count;
    uint32_t allocation_failures;
    uint32_t release_count;
} event_pool_t;

bool event_pool_init(event_pool_t *pool, size_t capacity);

event_t *event_pool_allocate(event_pool_t *pool, uint16_t signal);

bool event_pool_retain(event_pool_t *pool, event_t *event);

bool event_pool_release(event_pool_t *pool, event_t *event);

uint16_t event_pool_reference_count(const event_pool_t *pool, const event_t *event);

#endif
