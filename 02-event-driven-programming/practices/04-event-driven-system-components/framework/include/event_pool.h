#ifndef ED_EVENT_POOL_H
#define ED_EVENT_POOL_H

#include "event.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define ED_EVENT_POOL_CAPACITY (32U)

typedef struct
{
    ed_event_t events[ED_EVENT_POOL_CAPACITY];
    bool used[ED_EVENT_POOL_CAPACITY];
    size_t used_count;
    size_t high_water_mark;
    uint32_t allocation_count;
    uint32_t release_count;
    uint32_t allocation_failure_count;
    uint32_t invalid_release_count;
} ed_event_pool_t;

void ed_event_pool_init(ed_event_pool_t *pool);
ed_event_t *ed_event_pool_allocate(ed_event_pool_t *pool,
                                   ed_signal_t signal,
                                   uint8_t source,
                                   uint8_t destination);
bool ed_event_pool_retain(ed_event_pool_t *pool, ed_event_t *event);
bool ed_event_pool_release(ed_event_pool_t *pool, ed_event_t *event);
bool ed_event_pool_owns(const ed_event_pool_t *pool,
                        const ed_event_t *event);

#endif
