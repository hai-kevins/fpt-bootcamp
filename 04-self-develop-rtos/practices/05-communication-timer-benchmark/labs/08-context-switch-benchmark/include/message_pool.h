#ifndef HR_MESSAGE_POOL_H
#define HR_MESSAGE_POOL_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct hr_pool_block
{
    struct hr_pool_block * next;
} hr_pool_block_t;

typedef struct
{
    const char *name;
    uint8_t *memory;
    size_t block_size;
    size_t block_count;
    size_t free_count;
    hr_pool_block_t *free_list;
    uint8_t *allocation_map;
    uint32_t allocation_count;
    uint32_t free_operation_count;
    uint32_t failure_count;
} hr_message_pool_t;

bool hr_message_pool_init_static(hr_message_pool_t *pool, const char *name, void *memory, size_t block_size, size_t block_count,
    uint8_t *allocation_map);
void *hr_message_pool_alloc(hr_message_pool_t *pool);
bool hr_message_pool_free(hr_message_pool_t *pool, void *block);
bool hr_message_pool_owns(const hr_message_pool_t *pool, const void *block);
bool hr_message_pool_validate(const hr_message_pool_t *pool);

#endif
