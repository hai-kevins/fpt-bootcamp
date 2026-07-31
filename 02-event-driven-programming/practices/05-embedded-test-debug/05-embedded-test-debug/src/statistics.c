#include "statistics.h"

void statistics_capture(
    statistics_snapshot_t *snapshot,
    const event_queue_t *queue,
    const event_pool_t *pool
)
{
    if ((snapshot == NULL) ||
        (queue == NULL) ||
        (pool == NULL))
    {
        return;
    }

    snapshot->queue_count = queue->count;
    snapshot->queue_high_water =
        queue->high_water_mark;
    snapshot->queue_overflow =
        queue->overflow_count;

    snapshot->pool_used = pool->used_count;
    snapshot->pool_high_water =
        pool->high_water_mark;
    snapshot->pool_failures =
        pool->allocation_failures;
}
