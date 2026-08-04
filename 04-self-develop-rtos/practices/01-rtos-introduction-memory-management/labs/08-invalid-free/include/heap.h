#ifndef HEAP_H
#define HEAP_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef enum
{
    HEAP_STATUS_OK = 0,
    HEAP_STATUS_INVALID_ARGUMENT,
    HEAP_STATUS_NOT_INITIALIZED,
    HEAP_STATUS_POINTER_OUT_OF_RANGE,
    HEAP_STATUS_INVALID_POINTER,
    HEAP_STATUS_DOUBLE_FREE,
    HEAP_STATUS_CORRUPTED
} heap_status_t;

typedef struct
{
    size_t heap_size;
    size_t used_payload_bytes;
    size_t free_payload_bytes;
    size_t largest_free_block;
    size_t internal_fragmentation_bytes;
    size_t block_count;
    size_t free_block_count;
    size_t allocation_count;
    size_t free_count;
    size_t failed_allocation_count;
} heap_stats_t;

heap_status_t heap_init(void *memory, size_t size);
void *heap_alloc(size_t size);
heap_status_t heap_free(void *pointer);
bool heap_validate(void);
heap_status_t heap_get_stats(heap_stats_t *stats);
size_t heap_dump_blocks(size_t *sizes, bool *free_flags, size_t capacity);

#endif
