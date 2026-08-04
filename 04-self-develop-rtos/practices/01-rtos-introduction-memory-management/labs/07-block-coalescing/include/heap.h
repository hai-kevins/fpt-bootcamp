#ifndef LAB07_HEAP_H
#define LAB07_HEAP_H
#include <stdbool.h>
#include <stddef.h>

bool heap_init(void *memory, size_t size);
void *heap_alloc(size_t size);
bool heap_free(void *pointer);
size_t heap_block_count(void);
bool heap_validate(void);
#endif
