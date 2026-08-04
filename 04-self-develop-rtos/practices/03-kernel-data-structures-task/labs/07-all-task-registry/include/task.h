#ifndef RTOS_TASK_H
#define RTOS_TASK_H

#include "list.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define RTOS_TASK_MAGIC                 (0x5441534BUL)
#define RTOS_TASK_STACK_FILL            (0xA5A5A5A5UL)
#define RTOS_TASK_STACK_GUARD           (0xDEADBEEFUL)
#define RTOS_WAIT_FOREVER               (0xFFFFFFFFUL)
#define RTOS_DEFAULT_TIME_SLICE_TICKS   (5U)
#define RTOS_MIN_STACK_WORDS            (40U)

typedef void (*rtos_task_entry_t)(void *argument);

typedef enum
{
    RTOS_TASK_UNUSED = 0,
    RTOS_TASK_CREATED,
    RTOS_TASK_READY,
    RTOS_TASK_RUNNING,
    RTOS_TASK_BLOCKED,
    RTOS_TASK_SUSPENDED
} rtos_task_state_t;

typedef struct rtos_task
{
    uint32_t *saved_sp;
    uint32_t *stack_low;
    uint32_t *stack_high;
    size_t stack_word_count;
    const char *name;
    rtos_task_entry_t entry;
    void *argument;
    uint32_t wake_tick;
    uint32_t runtime_ticks;
    uint32_t switch_count;
    uint32_t time_slice_remaining;
    uint32_t magic;
    rtos_list_node_t ready_node;
    rtos_list_node_t all_task_node;
    uint8_t id;
    uint8_t base_priority;
    uint8_t effective_priority;
    rtos_task_state_t state;
} rtos_task_t;

bool rtos_task_create_static(rtos_task_t *task, const char *name, uint8_t id, uint8_t priority, rtos_task_entry_t entry,
    void *argument, uint32_t *stack, size_t stack_word_count);
bool rtos_task_is_valid(const rtos_task_t *task);
bool rtos_task_saved_sp_is_valid(const rtos_task_t *task);
size_t rtos_task_stack_unused_words(const rtos_task_t *task);
bool rtos_task_stack_guard_ok(const rtos_task_t *task);
void rtos_task_return_error(void);

#endif
