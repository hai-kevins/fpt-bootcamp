#ifndef RTOS_TASK_REGISTRY_H
#define RTOS_TASK_REGISTRY_H

#include "task.h"
#include <stdbool.h>
#include <stddef.h>

void rtos_task_registry_init(void);
bool rtos_task_registry_add(rtos_task_t *task);
size_t rtos_task_registry_count(void);
rtos_task_t *rtos_task_registry_at(size_t index);
rtos_task_t *rtos_task_registry_find(const char *name);
bool rtos_task_registry_validate(void);
const rtos_list_t *rtos_task_registry_list(void);

#endif
