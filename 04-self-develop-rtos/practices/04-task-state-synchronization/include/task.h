#ifndef RTOS_TASK_H
#define RTOS_TASK_H

#include "list.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define RTOS_TASK_MAGIC                 (0x5441534BUL)
#define RTOS_TASK_STACK_FILL            (0xA5A5A5A5UL)
#define RTOS_TASK_STACK_GUARD           (0xDEADBEEFUL)
#define RTOS_NO_WAIT                    (0U)
#define RTOS_WAIT_FOREVER               (0xFFFFFFFFUL)
#define RTOS_DEFAULT_TIME_SLICE_TICKS   (5U)
#define RTOS_MIN_STACK_WORDS            (48U)

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

typedef enum
{
    RTOS_WAIT_NONE = 0,
    RTOS_WAIT_SUCCESS,
    RTOS_WAIT_TIMEOUT,
    RTOS_WAIT_CANCELLED,
    RTOS_WAIT_SUSPENDED
} rtos_wait_result_t;

typedef enum
{
    RTOS_WAIT_KIND_NONE = 0,
    RTOS_WAIT_KIND_DELAY,
    RTOS_WAIT_KIND_SEMAPHORE,
    RTOS_WAIT_KIND_MUTEX
} rtos_wait_kind_t;

typedef struct rtos_task
{
    uint32_t *saved_sp;
    uint32_t *stack_low;
    uint32_t *stack_high;
    size_t stack_word_count;
    const char *name;
    rtos_task_entry_t entry;
    void *argument;
    void *waiting_object;
    rtos_list_t *waiting_list;
    uint32_t wake_tick;
    uint32_t runtime_ticks;
    uint32_t switch_count;
    uint32_t time_slice_remaining;
    uint32_t magic;
    rtos_list_node_t ready_node;
    rtos_list_node_t wait_node;
    rtos_list_node_t timeout_node;
    rtos_list_node_t all_task_node;
    rtos_list_t owned_mutexes;
    uint8_t id;
    uint8_t base_priority;
    uint8_t effective_priority;
    rtos_task_state_t state;
    rtos_wait_result_t wait_result;
    rtos_wait_kind_t wait_kind;
} rtos_task_t;

bool rtos_task_create_static(rtos_task_t *task,
                           const char *name,
                           uint8_t id,
                           uint8_t priority,
                           rtos_task_entry_t entry,
                           void *argument,
                           uint32_t *stack,
                           size_t stack_word_count);
bool rtos_task_is_valid(const rtos_task_t *task);
bool rtos_task_saved_sp_is_valid(const rtos_task_t *task);
size_t rtos_task_stack_unused_words(const rtos_task_t *task);
bool rtos_task_stack_guard_ok(const rtos_task_t *task);
bool rtos_task_transition_allowed(rtos_task_state_t from, rtos_task_state_t to);
const char *rtos_task_state_name(rtos_task_state_t state);
const char *rtos_wait_result_name(rtos_wait_result_t result);
void rtos_task_return_error(void);

#endif
