#ifndef RTOS_READY_QUEUE_H
#define RTOS_READY_QUEUE_H

#include "task.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define RTOS_PRIORITY_COUNT (4U)

void rtos_ready_init(void);
bool rtos_ready_insert(rtos_task_t *task);
bool rtos_ready_remove(rtos_task_t *task);
bool rtos_ready_rotate(uint8_t priority);
rtos_task_t *rtos_ready_select_highest(void);
size_t rtos_ready_count(uint8_t priority);
uint32_t rtos_ready_bitmap(void);
const rtos_list_t *rtos_ready_list(uint8_t priority);
bool rtos_ready_validate(void);

#endif
