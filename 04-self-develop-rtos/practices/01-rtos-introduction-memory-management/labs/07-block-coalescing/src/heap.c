#include "heap.h"
#include <stdint.h>

#define ALIGNMENT (8U)
#define MIN_PAYLOAD (8U)

typedef struct heap_block
{
    size_t payload_size;
    struct heap_block * previous;
    struct heap_block * next;
    bool is_free;
} heap_block_t;

static uint8_t *g_begin;
static uint8_t *g_end;
static heap_block_t *g_first;

static bool align_up(size_t value, size_t *result)
{
    if ((result == (size_t *)0) || (value > SIZE_MAX - (ALIGNMENT - 1U)))
    {
        return false;
    }
    *result = (value + ALIGNMENT - 1U) & ~(ALIGNMENT - 1U);
    return true;
}

static void *payload(heap_block_t *block)
{
    return (void *)((uint8_t *) block + sizeof(heap_block_t));
}

bool heap_init(void *memory, size_t size)
{
    uintptr_t raw = (uintptr_t) memory;
    uintptr_t aligned = (raw + ALIGNMENT - 1U) & ~(uintptr_t)(ALIGNMENT - 1U);
    size_t skipped = (size_t)(aligned - raw);
    if ((memory == (void *)0) || (size <= skipped + sizeof(heap_block_t)+MIN_PAYLOAD))
    {
        g_first = (heap_block_t *)0;
        return false;
    }
    g_begin = (uint8_t *) aligned;
    g_end = (uint8_t *) memory + size;
    g_first = (heap_block_t *) g_begin;
    g_first->payload_size = (size_t)(g_end - g_begin) - sizeof(heap_block_t);
    g_first->payload_size &= ~ (size_t)(ALIGNMENT - 1U);
    g_first->previous = (heap_block_t *)0;
    g_first->next = (heap_block_t *)0;
    g_first->is_free = true;
    g_end = (uint8_t *) payload(g_first) + g_first->payload_size;
    return true;
}

static heap_block_t *find_fit(size_t requested)
{
    heap_block_t *block = g_first;
    while (block != (heap_block_t *)0)
    {
        if (block->is_free && block->payload_size >= requested)
        {
            return block;
        }
        block = block->next;
    }
    return (heap_block_t *)0;
}

static void split(heap_block_t *block, size_t requested)
{
    if (block->payload_size < requested + sizeof(heap_block_t)+MIN_PAYLOAD)
    {
        return;
    }
    heap_block_t *new_block = (heap_block_t *)((uint8_t *) payload(block) + requested);
    new_block->payload_size = block->payload_size - requested - sizeof(heap_block_t);
    new_block->previous = block;
    new_block->next = block->next;
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
    if ((g_first == (heap_block_t *)0) || (size == 0U) || !align_up(size, &requested))
    {
        return (void *)0;
    }
    block = find_fit(requested);
    if (block == (heap_block_t *)0)
    {
        return (void *)0;
    }
    split(block, requested);
    block->is_free = false;
    return payload(block);
}

static void merge_next(heap_block_t *block)
{
    heap_block_t *next = block->next;
    if ((next == (heap_block_t *)0) || !block->is_free || !next->is_free)
    {
        return;
    }
    block->payload_size += sizeof(heap_block_t)+next->payload_size;
    block->next = next->next;
    if (block->next != (heap_block_t *)0)
    {
        block->next->previous = block;
    }
}

bool heap_free(void *pointer)
{
    heap_block_t *block = g_first;
    while (block != (heap_block_t *)0)
    {
        if (payload(block) == pointer)
        {
            if (block->is_free)
            {
                return false;
            }
            block->is_free = true;
            merge_next(block);
            if ((block->previous != (heap_block_t *)0) && block->previous->is_free)
            {
                block = block->previous;
                merge_next(block);
            }
            return true;
        }
        block = block->next;
    }
    return false;
}

size_t heap_block_count(void)
{
    size_t count = 0U;
    heap_block_t *block = g_first;
    while (block != (heap_block_t *)0)
    {
        count++;
        block = block->next;
    }
    return count;
}

bool heap_validate(void)
{
    const heap_block_t *block = g_first;
    const heap_block_t *previous = (const heap_block_t *)0;
    const uint8_t *expected = g_begin;
    while (block != (const heap_block_t *)0)
    {
        if (((const uint8_t *) block != expected) || (block->previous != previous))
        {
            return false;
        }
        expected = (const uint8_t *) block + sizeof(heap_block_t)+block->payload_size;
        if (expected > g_end)
        {
            return false;
        }
        previous = block;
        block = block->next;
    }
    return expected == g_end;
}
