#ifndef HR_MUTEX_H
#define HR_MUTEX_H

#include "task.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct hr_mutex
{
    const char *name;
    hr_task_t *owner;
    hr_list_t waiters;
    hr_list_node_t owner_node;
} hr_mutex_t;

bool hr_mutex_init(hr_mutex_t *mutex, const char *name);
hr_wait_result_t hr_mutex_lock(hr_mutex_t *mutex, uint32_t timeout_ticks);
bool hr_mutex_unlock(hr_mutex_t *mutex);
void hr_mutex_waiter_removed_locked(void *object);
bool hr_mutex_validate(const hr_mutex_t *mutex);

#endif
