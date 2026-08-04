#ifndef TASK_LAYOUT_H
#define TASK_LAYOUT_H

#include "list.h"
#include <stddef.h>
#include <stdint.h>

typedef enum
{
    TASK_UNUSED,
    TASK_CREATED,
    TASK_READY,
    TASK_RUNNING,
    TASK_BLOCKED
} task_state_t;

typedef struct task
{
    uint32_t *saved_sp;
    uint32_t *stack_low;
    uint32_t *stack_high;
    const char *name;
    uint8_t base_priority;
    uint8_t effective_priority;
    task_state_t state;
    rtos_list_node_t ready_node;
    rtos_list_node_t all_task_node;
    uint32_t magic;
} task_t;

_Static_assert(offsetof(task_t, saved_sp) == 0U, "saved_sp offset must remain zero");

#endif
