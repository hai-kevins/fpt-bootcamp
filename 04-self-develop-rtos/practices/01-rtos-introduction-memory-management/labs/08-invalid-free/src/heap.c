#include "heap.h"
#include <stdint.h>

#define HEAP_ALIGNMENT (8U)
#define HEAP_MIN_PAYLOAD (8U)
#define HEAP_MAGIC (0x48454150UL)
#define HEAP_FREED_MAGIC (0x46524545UL)

typedef struct heap_block
{
    size_t payload_size;
    size_t requested_size;
    struct heap_block * previous;
    struct heap_block * next;
    uint32_t magic;
    bool is_free;
} heap_block_t;

typedef struct
{
    uint8_t *begin;
    uint8_t *end;
    heap_block_t *first;
    bool initialized;
    size_t allocation_count;
    size_t free_count;
    size_t failed_allocation_count;
} heap_context_t;

static heap_context_t g_heap;

static bool align_up(size_t value, size_t *result)
{
    if ((result == (size_t *)0) || (value > SIZE_MAX - (HEAP_ALIGNMENT - 1U)))
    {
        return false;
    }
    *result = (value + HEAP_ALIGNMENT - 1U) & ~(HEAP_ALIGNMENT - 1U);
    return true;
}

static void *block_payload(heap_block_t *block)
{
    return (void *)((uint8_t *) block + sizeof(heap_block_t));
}

static const void *block_payload_const(const heap_block_t *block)
{
    return (const void *)((const uint8_t *) block + sizeof(heap_block_t));
}

heap_status_t heap_init(void *memory, size_t size)
{
    uintptr_t raw;
    uintptr_t aligned;
    size_t skipped;
    size_t available;
    if (memory == (void *)0)
    {
        return HEAP_STATUS_INVALID_ARGUMENT;
    }
    raw = (uintptr_t) memory;
    aligned = (raw + HEAP_ALIGNMENT - 1U) & ~(uintptr_t)(HEAP_ALIGNMENT - 1U);
    skipped = (size_t)(aligned - raw);
    if (size <= skipped + sizeof(heap_block_t)+HEAP_MIN_PAYLOAD)
    {
        return HEAP_STATUS_INVALID_ARGUMENT;
    }
    available = size - skipped;
    g_heap.begin = (uint8_t *) aligned;
    g_heap.first = (heap_block_t *) g_heap.begin;
    g_heap.first->payload_size = (available - sizeof(heap_block_t)) & ~(size_t)(HEAP_ALIGNMENT - 1U);
    g_heap.first->requested_size = 0U;
    g_heap.first->previous = (heap_block_t *)0;
    g_heap.first->next = (heap_block_t *)0;
    g_heap.first->magic = HEAP_MAGIC;
    g_heap.first->is_free = true;
    g_heap.end = (uint8_t *) block_payload(g_heap.first) + g_heap.first->payload_size;
    g_heap.initialized = true;
    g_heap.allocation_count = 0U;
    g_heap.free_count = 0U;
    g_heap.failed_allocation_count = 0U;
    return HEAP_STATUS_OK;
}

static heap_block_t *find_first_fit(size_t requested)
{
    heap_block_t *block = g_heap.first;
    while (block != (heap_block_t *)0)
    {
        if (block->magic != HEAP_MAGIC)
        {
            return (heap_block_t *)0;
        }
        if (block->is_free && block->payload_size >= requested)
        {
            return block;
        }
        block = block->next;
    }
    return (heap_block_t *)0;
}

static bool can_split(const heap_block_t *block, size_t requested)
{
    if (requested > SIZE_MAX - sizeof(heap_block_t)-HEAP_MIN_PAYLOAD)
    {
        return false;
    }
    return block->payload_size >= requested + sizeof(heap_block_t)+HEAP_MIN_PAYLOAD;
}

static void split_block(heap_block_t *block, size_t requested)
{
    heap_block_t *new_block;
    if (!can_split(block, requested))
    {
        return;
    }
    new_block = (heap_block_t *)((uint8_t *) block_payload(block) + requested);
    new_block->payload_size = block->payload_size - requested - sizeof(heap_block_t);
    new_block->requested_size = 0U;
    new_block->previous = block;
    new_block->next = block->next;
    new_block->magic = HEAP_MAGIC;
    new_block->is_free = true;
    if (new_block->next != (heap_block_t *)0)
    {
        new_block->next->previous = new_block;
    }
    block->payload_size = requested;
    block->next = new_block;
}

void *heap_alloc(size_t size)
{
    size_t requested;
    heap_block_t *block;
    if (!g_heap.initialized || (size == 0U) || !align_up(size, &requested))
    {
        if (g_heap.initialized)
        {
            g_heap.failed_allocation_count++;
        }
        return (void *)0;
    }
    block = find_first_fit(requested);
    if (block == (heap_block_t *)0)
    {
        g_heap.failed_allocation_count++;
        return (void *)0;
    }
    split_block(block, requested);
    block->requested_size = size;
    block->is_free = false;
    g_heap.allocation_count++;
    return block_payload(block);
}

static bool blocks_are_adjacent(const heap_block_t *left, const heap_block_t *right)
{
    return ((const uint8_t *) block_payload_const(left) + left->payload_size) == (const uint8_t *) right;
}

static heap_status_t merge_next(heap_block_t *block)
{
    heap_block_t *next;
    if ((block == (heap_block_t *)0) || !block->is_free)
    {
        return HEAP_STATUS_OK;
    }
    next = block->next;
    if ((next == (heap_block_t *)0) || !next->is_free)
    {
        return HEAP_STATUS_OK;
    }
    if ((block->magic != HEAP_MAGIC) || (next->magic != HEAP_MAGIC) || !blocks_are_adjacent(block, next))
    {
        return HEAP_STATUS_CORRUPTED;
    }
    block->payload_size += sizeof(heap_block_t)+next->payload_size;
    block->next = next->next;
    if (block->next != (heap_block_t *)0)
    {
        block->next->previous = block;
    }
    next->magic = HEAP_FREED_MAGIC;
    next->previous = (heap_block_t *)0;
    next->next = (heap_block_t *)0;
    next->payload_size = 0U;
    next->requested_size = 0U;
    return HEAP_STATUS_OK;
}

static heap_block_t *find_block_by_payload(void *pointer)
{
    heap_block_t *block = g_heap.first;
    while (block != (heap_block_t *)0)
    {
        if (block->magic != HEAP_MAGIC)
        {
            return (heap_block_t *)0;
        }
        if (block_payload(block) == pointer)
        {
            return block;
        }
        block = block->next;
    }
    return (heap_block_t *)0;
}

heap_status_t heap_free(void *pointer)
{
    heap_block_t *block;
    heap_status_t status;
    if (!g_heap.initialized)
    {
        return HEAP_STATUS_NOT_INITIALIZED;
    }
    if (pointer == (void *)0)
    {
        return HEAP_STATUS_INVALID_ARGUMENT;
    }
    if (((uint8_t *) pointer < g_heap.begin + sizeof(heap_block_t)) || ((uint8_t *) pointer >= g_heap.end))
    {
        return HEAP_STATUS_POINTER_OUT_OF_RANGE;
    }
    block = find_block_by_payload(pointer);
    if (block == (heap_block_t *)0)
    {
        return HEAP_STATUS_INVALID_POINTER;
    }
    if (block->is_free)
    {
        return HEAP_STATUS_DOUBLE_FREE;
    }
    block->is_free = true;
    block->requested_size = 0U;
    g_heap.free_count++;
    status = merge_next(block);
    if (status != HEAP_STATUS_OK)
    {
        return status;
    }
    if ((block->previous != (heap_block_t *)0) && block->previous->is_free)
    {
        block = block->previous;
        status = merge_next(block);
    }
    return status;
}

bool heap_validate(void)
{
    const heap_block_t *block;
    const heap_block_t *previous;
    const uint8_t *expected;
    if (!g_heap.initialized)
    {
        return false;
    }
    block = g_heap.first;
    previous = (const heap_block_t *)0;
    expected = g_heap.begin;
    while (block != (const heap_block_t *)0)
    {
        if (((const uint8_t *) block != expected) || (block->previous != previous) || (block->magic != HEAP_MAGIC))
        {
            return false;
        }
        if ((((uintptr_t) block_payload_const(block)) & (HEAP_ALIGNMENT - 1U)) != 0U)
        {
            return false;
        }
        if (!block->is_free && ((block->requested_size == 0U) || (block->requested_size > block->payload_size)))
        {
            return false;
        }
        expected = (const uint8_t *) block_payload_const(block) + block->payload_size;
        if (expected > g_heap.end)
        {
            return false;
        }
        if (block->is_free && (block->next != (const heap_block_t *)0) && block->next->is_free)
        {
            return false;
        }
        previous = block;
        block = block->next;
    }
    return expected == g_heap.end;
}

heap_status_t heap_get_stats(heap_stats_t *stats)
{
    heap_block_t *block;
    if (!g_heap.initialized)
    {
        return HEAP_STATUS_NOT_INITIALIZED;
    }
    if (stats == (heap_stats_t *)0)
    {
        return HEAP_STATUS_INVALID_ARGUMENT;
    }
    stats->heap_size = (size_t)(g_heap.end - g_heap.begin);
    stats->used_payload_bytes = 0U;
    stats->free_payload_bytes = 0U;
    stats->largest_free_block = 0U;
    stats->internal_fragmentation_bytes = 0U;
    stats->block_count = 0U;
    stats->free_block_count = 0U;
    stats->allocation_count = g_heap.allocation_count;
    stats->free_count = g_heap.free_count;
    stats->failed_allocation_count = g_heap.failed_allocation_count;
    block = g_heap.first;
    while (block != (heap_block_t *)0)
    {
        if (block->magic != HEAP_MAGIC)
        {
            return HEAP_STATUS_CORRUPTED;
        }
        stats->block_count++;
        if (block->is_free)
        {
            stats->free_block_count++;
            stats->free_payload_bytes += block->payload_size;
            if (block->payload_size > stats->largest_free_block)
            {
                stats->largest_free_block = block->payload_size;
            }
        }
        else
        {
            stats->used_payload_bytes += block->payload_size;
            stats->internal_fragmentation_bytes += block->payload_size - block->requested_size;
        }
        block = block->next;
    }
    return HEAP_STATUS_OK;
}

size_t heap_dump_blocks(size_t *sizes, bool *free_flags, size_t capacity)
{
    size_t count = 0U;
    heap_block_t *block = g_heap.first;
    while (block != (heap_block_t *)0)
    {
        if ((count < capacity) && (sizes != (size_t *)0))
        {
            sizes[count] = block->payload_size;
        }
        if ((count < capacity) && (free_flags != (bool *)0))
        {
            free_flags[count] = block->is_free;
        }
        count++;
        block = block->next;
    }
    return count;
}
