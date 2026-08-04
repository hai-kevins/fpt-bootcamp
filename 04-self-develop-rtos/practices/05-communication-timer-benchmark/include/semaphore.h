#ifndef HR_SEMAPHORE_H
#define HR_SEMAPHORE_H

#include "task.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct
{
    uint32_t count;
    uint32_t max_count;
    hr_list_t waiters;
    const char *name;
} hr_semaphore_t;

bool hr_semaphore_init(hr_semaphore_t *semaphore, const char *name, uint32_t initial_count, uint32_t max_count);
hr_wait_result_t hr_semaphore_take(hr_semaphore_t *semaphore, uint32_t timeout_ticks);
bool hr_semaphore_give(hr_semaphore_t *semaphore);
bool hr_semaphore_give_from_isr(hr_semaphore_t *semaphore, bool *higher_priority_task_woken);
bool hr_semaphore_validate(const hr_semaphore_t *semaphore);

#endif
