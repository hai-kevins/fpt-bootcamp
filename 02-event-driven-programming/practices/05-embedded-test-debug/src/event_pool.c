#include "event_pool.h"

#include <string.h>

static event_pool_block_t *find_block(event_pool_t *pool, const event_t *event)
{
    if ((pool == NULL) || (event == NULL))
    {
        return NULL;
    }

    for (size_t i = 0U; i < pool->capacity; i++)
    {
        if (&pool->blocks[i].event == event)
        {
            return &pool->blocks[i];
        }
    }

    return NULL;
}

static const event_pool_block_t *find_block_const(const event_pool_t *pool, const event_t *event)
{
    if ((pool == NULL) || (event == NULL))
    {
        return NULL;
    }

    for (size_t i = 0U; i < pool->capacity; i++)
    {
        if (&pool->blocks[i].event == event)
        {
            return &pool->blocks[i];
        }
    }

    return NULL;
}

bool event_pool_init(event_pool_t *pool, size_t capacity)
{
    if ((pool == NULL) || (capacity == 0U) || (capacity > EVENT_POOL_MAX_BLOCKS))
    {
        return false;
    }

    (void) memset(pool, 0, sizeof (*pool));
    pool->capacity = capacity;
    return true;
}

event_t *event_pool_allocate(event_pool_t *pool, uint16_t signal)
{
    if (pool == NULL)
    {
        return NULL;
    }

    for (size_t i = 0U; i < pool->capacity; i++)
    {
        event_pool_block_t *block = &pool->blocks[i];

        if (!block->used)
        {
            (void) memset(&block->event, 0, sizeof(block->event));
            block->event.signal = signal;
            block->reference_count = 1U;
            block->used = true;

            pool->used_count++;
            pool->allocation_count++;

            if (pool->used_count > pool->high_water_mark)
            {
                pool->high_water_mark = pool->used_count;
            }

            return &block->event;
        }
    }

    pool->allocation_failures++;
    return NULL;
}

bool event_pool_retain(event_pool_t *pool, event_t *event)
{
    event_pool_block_t *block = find_block(pool, event);

    if ((block == NULL) || (!block->used) || (block->reference_count == UINT16_MAX))
    {
        return false;
    }

    block->reference_count++;
    return true;
}

bool event_pool_release(event_pool_t *pool, event_t *event)
{
    event_pool_block_t *block = find_block(pool, event);

    if ((block == NULL) || (!block->used) || (block->reference_count == 0U))
    {
        return false;
    }

    block->reference_count--;

    if (block->reference_count == 0U)
    {
        block->used = false;
        (void) memset(&block->event, 0, sizeof(block->event));
        pool->used_count--;
        pool->release_count++;
    }

    return true;
}

uint16_t event_pool_reference_count(const event_pool_t *pool, const event_t *event)
{
    const event_pool_block_t *block = find_block_const(pool, event);

    if ((block == NULL) || (!block->used))
    {
        return 0U;
    }

    return block->reference_count;
}
