#ifndef STATISTICS_H
#define STATISTICS_H

#include "event_pool.h"
#include "event_queue.h"

#include <stddef.h>
#include <stdint.h>

typedef struct
{
    size_t queue_count;
    size_t queue_high_water;
    uint32_t queue_overflow;

    size_t pool_used;
    size_t pool_high_water;
    uint32_t pool_failures;
} statistics_snapshot_t;

void statistics_capture(statistics_snapshot_t *snapshot, const event_queue_t *queue, const event_pool_t *pool);

#endif
