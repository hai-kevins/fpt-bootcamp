#ifndef HR_READY_QUEUE_H
#define HR_READY_QUEUE_H

#include "task.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define HR_PRIORITY_COUNT (4U)

void hr_ready_init(void);
bool hr_ready_insert(hr_task_t *task);
bool hr_ready_remove(hr_task_t *task);
bool hr_ready_rotate(uint8_t priority);
hr_task_t *hr_ready_select_highest(void);
size_t hr_ready_count(uint8_t priority);
uint32_t hr_ready_bitmap(void);
const hr_list_t *hr_ready_list(uint8_t priority);
bool hr_ready_validate(void);

#endif
