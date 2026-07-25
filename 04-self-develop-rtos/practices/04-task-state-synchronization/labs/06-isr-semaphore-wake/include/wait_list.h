#ifndef RTOS_WAIT_LIST_H
#define RTOS_WAIT_LIST_H

#include "task.h"
#include <stdbool.h>

void rtos_wait_list_init(rtos_list_t *list);
bool rtos_wait_list_insert(rtos_list_t *list, rtos_task_t *task);
bool rtos_wait_list_remove(rtos_list_t *list, rtos_task_t *task);
rtos_task_t *rtos_wait_list_front(const rtos_list_t *list);
bool rtos_wait_list_validate(const rtos_list_t *list);

#endif
