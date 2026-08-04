#include "message_pool.h"
#include "critical_section.h"
#include <stdint.h>

static size_t align_up(size_t value, size_t alignment)
{
    return (value + alignment - 1U) & ~(alignment - 1U);
}

static bool block_index(const hr_message_pool_t *pool, const void *block, size_t *index)
{
    const uintptr_t start = (uintptr_t) pool->memory;
    const uintptr_t end = start + (pool->block_size * pool->block_count);
    const uintptr_t value = (uintptr_t) block;
    if ((value < start) || (value >= end) || (((value - start) % pool->block_size) != 0U))
    {
        return false;
    }
    if (index != (size_t *)0)
    {
        *index = (size_t)((value - start) / pool->block_size);
    }
    return true;
}

bool hr_message_pool_init_static(hr_message_pool_t *pool, const char *name, void *memory, size_t block_size, size_t block_count,
    uint8_t *allocation_map)
{
    size_t aligned_size;
    if ((pool == (hr_message_pool_t *)0) || (name == (const char *)0) || (memory == (void *)0) || (allocation_map == (uint8_t *)0)
        || (block_count == 0U) || (block_size < sizeof(hr_pool_block_t)))
    {
        return false;
    }
    aligned_size = align_up(block_size, sizeof(void *));
    if ((aligned_size < block_size) || (block_count > (SIZE_MAX / aligned_size)))
    {
        return false;
    }
    pool->name = name;
    pool->memory = (uint8_t *) memory;
    pool->block_size = aligned_size;
    pool->block_count = block_count;
    pool->free_count = block_count;
    pool->free_list = (hr_pool_block_t *)0;
    pool->allocation_map = allocation_map;
    pool->allocation_count = 0U;
    pool->free_operation_count = 0U;
    pool->failure_count = 0U;

    for (size_t i = block_count; i > 0U; --i)
    {
        hr_pool_block_t *block = (hr_pool_block_t *)(void *)(pool->memory + ((i - 1U) * aligned_size));
        block->next = pool->free_list;
        pool->free_list = block;
        allocation_map[i - 1U] = 0U;
    }
    return true;
}

void *hr_message_pool_alloc(hr_message_pool_t *pool)
{
    hr_pool_block_t *block;
    size_t index;
    const hr_irq_state_t state = hr_critical_enter();
    if ((pool == (hr_message_pool_t *)0) || (pool->free_list == (hr_pool_block_t *)0))
    {
        if (pool != (hr_message_pool_t *)0)
        {
            ++pool->failure_count;
        }
        hr_critical_exit(state);
        return (void *)0;
    }
    block = pool->free_list;
    pool->free_list = block->next;
    if (!block_index(pool, block, &index) || (pool->allocation_map[index] != 0U))
    {
        ++pool->failure_count;
        hr_critical_exit(state);
        return (void *)0;
    }
    pool->allocation_map[index] = 1U;
    --pool->free_count;
    ++pool->allocation_count;
    hr_critical_exit(state);
    return block;
}

bool hr_message_pool_free(hr_message_pool_t *pool, void *block)
{
    size_t index;
    hr_pool_block_t *node;
    const hr_irq_state_t state = hr_critical_enter();
    if ((pool == (hr_message_pool_t *)0) || !block_index(pool, block, &index) || (pool->allocation_map[index] == 0U))
    {
        if (pool != (hr_message_pool_t *)0)
        {
            ++pool->failure_count;
        }
        hr_critical_exit(state);
        return false;
    }
    pool->allocation_map[index] = 0U;
    node = (hr_pool_block_t *) block;
    node->next = pool->free_list;
    pool->free_list = node;
    ++pool->free_count;
    ++pool->free_operation_count;
    hr_critical_exit(state);
    return true;
}

bool hr_message_pool_owns(const hr_message_pool_t *pool, const void *block)
{
    size_t index;
    return (pool != (const hr_message_pool_t *)0) && block_index(pool, block, &index);
}

bool hr_message_pool_validate(const hr_message_pool_t *pool)
{
    size_t free_map = 0U;
    size_t free_list_count = 0U;
    const hr_pool_block_t *slow;
    const hr_pool_block_t *fast;
    if ((pool == (const hr_message_pool_t *)0) || (pool->name == (const char *)0) || (pool->memory == (uint8_t *)0)
        || (pool->allocation_map == (uint8_t *)0) || (pool->block_count == 0U) || (pool->free_count > pool->block_count))
    {
        return false;
    }
    for (size_t i = 0U; i < pool->block_count; ++i)
    {
        if (pool->allocation_map[i] == 0U)
        {
            ++free_map;
        }
        else if (pool->allocation_map[i] != 1U)
        {
            return false;
        }
    }
    slow = pool->free_list;
    fast = pool->free_list;
    while ((fast != (const hr_pool_block_t *)0) && (fast->next != (const hr_pool_block_t *)0))
    {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast)
        {
            return false;
        }
    }
    for (const hr_pool_block_t *node = pool->free_list; node != (const hr_pool_block_t *)0; node = node->next)
    {
        size_t index;
        if (!block_index(pool, node, &index) || (pool->allocation_map[index] != 0U))
        {
            return false;
        }
        ++free_list_count;
        if (free_list_count > pool->block_count)
        {
            return false;
        }
    }
    return (free_map == pool->free_count) && (free_list_count == pool->free_count);
}
