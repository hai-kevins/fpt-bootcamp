#ifndef HR_TIMEOUT_H
#define HR_TIMEOUT_H

#include "task.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void hr_timeout_init(void);
bool hr_tick_reached(uint32_t now, uint32_t deadline);
bool hr_tick_before(uint32_t left, uint32_t right);
bool hr_timeout_insert(hr_task_t *task);
bool hr_timeout_remove(hr_task_t *task);
hr_task_t *hr_timeout_front(void);
hr_task_t *hr_timeout_take_due(uint32_t now);
size_t hr_timeout_count(void);
const hr_list_t *hr_timeout_list(void);
bool hr_timeout_validate(void);

#endif
