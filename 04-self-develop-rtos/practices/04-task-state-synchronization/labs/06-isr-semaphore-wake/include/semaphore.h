#ifndef RTOS_SEMAPHORE_H
#define RTOS_SEMAPHORE_H

#include "task.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct
{
    uint32_t count;
    uint32_t max_count;
    rtos_list_t waiters;
    const char *name;
} rtos_semaphore_t;

bool rtos_semaphore_init(rtos_semaphore_t *semaphore, const char *name, uint32_t initial_count, uint32_t max_count);
rtos_wait_result_t rtos_semaphore_take(rtos_semaphore_t *semaphore, uint32_t timeout_ticks);
bool rtos_semaphore_give(rtos_semaphore_t *semaphore);
bool rtos_semaphore_give_from_isr(rtos_semaphore_t *semaphore, bool *higher_priority_task_woken);
bool rtos_semaphore_validate(const rtos_semaphore_t *semaphore);

#endif
