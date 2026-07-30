#include "event_pool.h"

#include <string.h>

void ed_event_pool_init(ed_event_pool_t *pool)
{
    if (pool != NULL)
    {
        (void)memset(pool, 0, sizeof(*pool));
    }
}

ed_event_t *ed_event_pool_allocate(ed_event_pool_t *pool,
                                   ed_signal_t signal,
                                   uint8_t source,
                                   uint8_t destination)
{
    if (pool == NULL)
    {
        return NULL;
    }

    for (size_t i = 0U; i < ED_EVENT_POOL_CAPACITY; i++)
    {
        if (!pool->used[i])
        {
            ed_event_t *event = &pool->events[i];
            (void)memset(event, 0, sizeof(*event));
            pool->used[i] = true;
            pool->used_count++;
            pool->allocation_count++;

            if (pool->used_count > pool->high_water_mark)
            {
                pool->high_water_mark = pool->used_count;
            }

            event->signal = signal;
            event->source = source;
            event->destination = destination;
            event->flags = ED_EVENT_FLAG_DYNAMIC;
            event->ref_count = 1U;
            event->pool_index = (uint8_t)i;
            return event;
        }
    }

    pool->allocation_failure_count++;
    return NULL;
}

bool ed_event_pool_owns(const ed_event_pool_t *pool,
                        const ed_event_t *event)
{
    if ((pool == NULL) || (event == NULL))
    {
        return false;
    }

    return (event >= &pool->events[0]) &&
           (event < &pool->events[ED_EVENT_POOL_CAPACITY]);
}

bool ed_event_pool_retain(ed_event_pool_t *pool, ed_event_t *event)
{
    if (!ed_event_pool_owns(pool, event) ||
        ((event->flags & ED_EVENT_FLAG_DYNAMIC) == 0U) ||
        (event->pool_index >= ED_EVENT_POOL_CAPACITY) ||
        !pool->used[event->pool_index] ||
        (event->ref_count == UINT8_MAX))
    {
        return false;
    }

    event->ref_count++;
    return true;
}

bool ed_event_pool_release(ed_event_pool_t *pool, ed_event_t *event)
{
    if (event == NULL)
    {
        return true;
    }

    if (!ed_event_pool_owns(pool, event))
    {
        return (event->flags & ED_EVENT_FLAG_DYNAMIC) == 0U;
    }

    if (((event->flags & ED_EVENT_FLAG_DYNAMIC) == 0U) ||
        (event->pool_index >= ED_EVENT_POOL_CAPACITY) ||
        !pool->used[event->pool_index] ||
        (event->ref_count == 0U))
    {
        pool->invalid_release_count++;
        return false;
    }

    event->ref_count--;
    if (event->ref_count == 0U)
    {
        const uint8_t index = event->pool_index;
        pool->used[index] = false;
        pool->used_count--;
        pool->release_count++;
        (void)memset(event, 0, sizeof(*event));
        event->pool_index = UINT8_MAX;
    }

    return true;
}
