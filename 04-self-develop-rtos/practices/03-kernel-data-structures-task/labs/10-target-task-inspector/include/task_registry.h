#ifndef HR_TASK_REGISTRY_H
#define HR_TASK_REGISTRY_H

#include "task.h"
#include <stdbool.h>
#include <stddef.h>

void hr_task_registry_init(void);
bool hr_task_registry_add(hr_task_t *task);
size_t hr_task_registry_count(void);
hr_task_t *hr_task_registry_at(size_t index);
hr_task_t *hr_task_registry_find(const char *name);
bool hr_task_registry_validate(void);
const hr_list_t *hr_task_registry_list(void);

#endif
