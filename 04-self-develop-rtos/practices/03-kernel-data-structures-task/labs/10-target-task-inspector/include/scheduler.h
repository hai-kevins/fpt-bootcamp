#ifndef HR_SCHEDULER_H
#define HR_SCHEDULER_H

#include "compiler.h"
#include "task.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

extern hr_task_t *g_current_task;
extern volatile uint32_t g_kernel_tick;
extern volatile uint32_t g_context_switch_count;

void hr_scheduler_init(void);
bool hr_scheduler_add_task(hr_task_t *task);
HR_NORETURN void hr_scheduler_start(void);
void hr_scheduler_commit_switch(void);
void hr_scheduler_on_tick(void);
void hr_task_yield(void);
void hr_task_delay(uint32_t ticks);
hr_task_t *hr_scheduler_current(void);
bool hr_scheduler_validate(void);

#endif
