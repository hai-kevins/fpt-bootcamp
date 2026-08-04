#ifndef ED_SCHEDULER_H
#define ED_SCHEDULER_H

#include "active_object.h"
#include "event_pool.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define ED_SCHEDULER_MAX_OBJECTS (16U)

typedef struct
{
    ed_active_object_t *objects[ED_SCHEDULER_MAX_OBJECTS];
    size_t count;
    size_t last_index;
    uint32_t dispatch_count;
    uint32_t idle_count;
    ed_event_pool_t *pool;
} ed_scheduler_t;

void ed_scheduler_init(ed_scheduler_t *scheduler, ed_event_pool_t *pool);
bool ed_scheduler_register(ed_scheduler_t *scheduler, ed_active_object_t *object);
bool ed_scheduler_run_once(ed_scheduler_t *scheduler);
size_t ed_scheduler_run_until_idle(ed_scheduler_t *scheduler, size_t budget);

#endif
