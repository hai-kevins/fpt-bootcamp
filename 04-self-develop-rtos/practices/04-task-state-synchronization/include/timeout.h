#ifndef RTOS_TIMEOUT_H
#define RTOS_TIMEOUT_H

#include "task.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void rtos_timeout_init(void);
bool rtos_tick_reached(uint32_t now, uint32_t deadline);
bool rtos_tick_before(uint32_t left, uint32_t right);
bool rtos_timeout_insert(rtos_task_t *task);
bool rtos_timeout_remove(rtos_task_t *task);
rtos_task_t *rtos_timeout_front(void);
rtos_task_t *rtos_timeout_take_due(uint32_t now);
size_t rtos_timeout_count(void);
const rtos_list_t *rtos_timeout_list(void);
bool rtos_timeout_validate(void);

#endif
