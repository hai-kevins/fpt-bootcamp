#include "heap.h"
#include <stdint.h>

#define HEAP_ALIGNMENT (8U)

typedef struct
{
    size_t payload_size;
    bool is_free;
} heap_block_t;

static heap_block_t *g_block;

static size_t align_up(size_t value)
{
    return (value + HEAP_ALIGNMENT - 1U) & ~(HEAP_ALIGNMENT - 1U);
}

bool heap_init(void *memory, size_t size)
{
    uintptr_t address = (uintptr_t) memory;
    uintptr_t aligned = (address + HEAP_ALIGNMENT - 1U) & ~(uintptr_t)(HEAP_ALIGNMENT - 1U);
    size_t skipped = (size_t)(aligned - address);

    if ((memory == (void *)0) || (size <= skipped + sizeof(heap_block_t)))
    {
        g_block = (heap_block_t *)0;
        return false;
    }

    g_block = (heap_block_t *) aligned;
    g_block->payload_size = size - skipped - sizeof(heap_block_t);
    g_block->payload_size &= ~ (size_t)(HEAP_ALIGNMENT - 1U);
    g_block->is_free = true;
    return g_block->payload_size > 0U;
}

void *heap_alloc(size_t size)
{
    size_t requested;

    if ((g_block == (heap_block_t *)0) || !g_block->is_free || (size == 0U))
    {
        return (void *)0;
    }

    requested = align_up(size);

    if (requested > g_block->payload_size)
    {
        return (void *)0;
    }

    g_block->is_free = false;
    return (void *)(g_block + 1);
}

bool heap_free(void *pointer)
{
    if ((g_block == (heap_block_t *)0) || (pointer != (void *)(g_block + 1)) || g_block->is_free)
    {
        return false;
    }

    g_block->is_free = true;
    return true;
}

size_t heap_capacity(void)
{
    return (g_block == (heap_block_t *)0) ? 0U : g_block->payload_size;
}

bool heap_is_free(void)
{
    return (g_block != (heap_block_t *)0) && g_block->is_free;
}
