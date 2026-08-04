#ifndef MEMORY_EXPLORER_H
#define MEMORY_EXPLORER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

bool memory_explorer_init(uint8_t *heap_memory, size_t heap_size);
void memory_explorer_print_help(void);
void memory_explorer_execute(const char *line);

#endif
