#ifndef INITIAL_STACK_H
#define INITIAL_STACK_H

#include <stddef.h>
#include <stdint.h>

typedef void (*lab_task_entry_t)(void *argument);

uint32_t *lab_build_initial_stack(uint32_t *stack, size_t word_count, lab_task_entry_t entry, void *argument, uintptr_t return_handler);

#endif
