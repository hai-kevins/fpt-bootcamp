#ifndef RTOS_MUTEX_H
#define RTOS_MUTEX_H

#include "task.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct rtos_mutex
{
    const char *name;
    rtos_task_t *owner;
    rtos_list_t waiters;
    rtos_list_node_t owner_node;
} rtos_mutex_t;

bool rtos_mutex_init(rtos_mutex_t *mutex, const char *name);
rtos_wait_result_t rtos_mutex_lock(rtos_mutex_t *mutex, uint32_t timeout_ticks);
bool rtos_mutex_unlock(rtos_mutex_t *mutex);
void rtos_mutex_waiter_removed_locked(void *object);
bool rtos_mutex_validate(const rtos_mutex_t *mutex);

#endif
