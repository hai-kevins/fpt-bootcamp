#ifndef TASK_H
#define TASK_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define RTOS_TASK_STACK_FILL  (0xA5A5A5A5UL)
#define RTOS_TASK_STACK_GUARD (0xDEADBEEFUL)
#define RTOS_WAIT_FOREVER     (0xFFFFFFFFUL)
#define RTOS_DEFAULT_TIME_SLICE_TICKS (5U)

typedef void (*rtos_task_entry_t)(void *argument);

typedef enum
{
    RTOS_TASK_CREATED = 0,
    RTOS_TASK_READY,
    RTOS_TASK_RUNNING,
    RTOS_TASK_BLOCKED
} rtos_task_state_t;

typedef struct rtos_task
{
    uint32_t *saved_sp;
    uint32_t *stack_low;
    uint32_t *stack_high;
    const char *name;
    rtos_task_entry_t entry;
    void *argument;
    struct rtos_task *ready_previous;
    struct rtos_task *ready_next;
    uint32_t wake_tick;
    uint32_t runtime_ticks;
    uint32_t switch_count;
    uint32_t time_slice_remaining;
    uint16_t stack_word_count;
    uint8_t id;
    uint8_t priority;
    rtos_task_state_t state;
    bool in_ready_queue;
} rtos_task_t;

bool rtos_task_create_static(rtos_task_t *task,
                           const char *name,
                           uint8_t id,
                           uint8_t priority,
                           rtos_task_entry_t entry,
                           void *argument,
                           uint32_t *stack,
                           size_t stack_word_count);

size_t rtos_task_stack_unused_words(const rtos_task_t *task);
bool rtos_task_stack_guard_ok(const rtos_task_t *task);
void rtos_task_return_error(void);

#endif
