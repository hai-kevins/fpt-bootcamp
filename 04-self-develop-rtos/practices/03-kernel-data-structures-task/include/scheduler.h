#ifndef RTOS_SCHEDULER_H
#define RTOS_SCHEDULER_H

#include "compiler.h"
#include "task.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

extern rtos_task_t *g_current_task;
extern volatile uint32_t g_kernel_tick;
extern volatile uint32_t g_context_switch_count;

void rtos_scheduler_init(void);
bool rtos_scheduler_add_task(rtos_task_t *task);
RTOS_NORETURN void rtos_scheduler_start(void);
void rtos_scheduler_commit_switch(void);
void rtos_scheduler_on_tick(void);
void rtos_task_yield(void);
void rtos_task_delay(uint32_t ticks);
rtos_task_t *rtos_scheduler_current(void);
bool rtos_scheduler_validate(void);

#endif
