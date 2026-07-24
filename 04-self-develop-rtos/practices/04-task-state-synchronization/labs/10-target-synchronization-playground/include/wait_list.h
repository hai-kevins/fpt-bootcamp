#ifndef HR_WAIT_LIST_H
#define HR_WAIT_LIST_H

#include "task.h"
#include <stdbool.h>

void hr_wait_list_init(hr_list_t *list);
bool hr_wait_list_insert(hr_list_t *list, hr_task_t *task);
bool hr_wait_list_remove(hr_list_t *list, hr_task_t *task);
hr_task_t *hr_wait_list_front(const hr_list_t *list);
bool hr_wait_list_validate(const hr_list_t *list);

#endif
