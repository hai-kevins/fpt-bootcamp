#ifndef HR_SCHEDULER_H
#define HR_SCHEDULER_H

#include "compiler.h"
#include "task.h"
#include <stdbool.h>
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
hr_wait_result_t hr_task_delay(uint32_t ticks);
hr_wait_result_t hr_task_delay_until(uint32_t *previous_wake, uint32_t period_ticks);
hr_task_t *hr_scheduler_current(void);
uint32_t hr_scheduler_tick_now(void);

bool hr_scheduler_block_current_locked(void *object, hr_list_t *waiters, hr_wait_kind_t kind, uint32_t timeout_ticks);
bool hr_scheduler_wake_task_locked(hr_task_t *task, hr_wait_result_t result);
bool hr_scheduler_set_effective_priority_locked(hr_task_t *task, uint8_t priority);
bool hr_task_suspend(hr_task_t *task);
bool hr_task_resume(hr_task_t *task);
bool hr_scheduler_validate(void);

#endif
