#ifndef HR_TASK_H
#define HR_TASK_H

#include "list.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define HR_TASK_MAGIC                 (0x5441534BUL)
#define HR_TASK_STACK_FILL            (0xA5A5A5A5UL)
#define HR_TASK_STACK_GUARD           (0xDEADBEEFUL)
#define HR_WAIT_FOREVER               (0xFFFFFFFFUL)
#define HR_DEFAULT_TIME_SLICE_TICKS   (5U)
#define HR_MIN_STACK_WORDS            (40U)

typedef void (*hr_task_entry_t)(void *argument);

typedef enum
{
    HR_TASK_UNUSED = 0,
    HR_TASK_CREATED,
    HR_TASK_READY,
    HR_TASK_RUNNING,
    HR_TASK_BLOCKED,
    HR_TASK_SUSPENDED
} hr_task_state_t;

typedef struct hr_task
{
    uint32_t *saved_sp;
    uint32_t *stack_low;
    uint32_t *stack_high;
    size_t stack_word_count;
    const char *name;
    hr_task_entry_t entry;
    void *argument;
    uint32_t wake_tick;
    uint32_t runtime_ticks;
    uint32_t switch_count;
    uint32_t time_slice_remaining;
    uint32_t magic;
    hr_list_node_t ready_node;
    hr_list_node_t all_task_node;
    uint8_t id;
    uint8_t base_priority;
    uint8_t effective_priority;
    hr_task_state_t state;
} hr_task_t;

bool hr_task_create_static(hr_task_t *task,
                           const char *name,
                           uint8_t id,
                           uint8_t priority,
                           hr_task_entry_t entry,
                           void *argument,
                           uint32_t *stack,
                           size_t stack_word_count);
bool hr_task_is_valid(const hr_task_t *task);
bool hr_task_saved_sp_is_valid(const hr_task_t *task);
size_t hr_task_stack_unused_words(const hr_task_t *task);
bool hr_task_stack_guard_ok(const hr_task_t *task);
void hr_task_return_error(void);

#endif
