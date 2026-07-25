#ifndef RTOS_SCHEDULER_H
#define RTOS_SCHEDULER_H

#include "compiler.h"
#include "task.h"
#include <stdbool.h>
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
rtos_wait_result_t rtos_task_delay(uint32_t ticks);
rtos_wait_result_t rtos_task_delay_until(uint32_t *previous_wake, uint32_t period_ticks);
rtos_task_t *rtos_scheduler_current(void);
uint32_t rtos_scheduler_tick_now(void);

bool rtos_scheduler_block_current_locked(void *object,
                                       rtos_list_t *waiters,
                                       rtos_wait_kind_t kind,
                                       uint32_t timeout_ticks);
bool rtos_scheduler_wake_task_locked(rtos_task_t *task, rtos_wait_result_t result);
bool rtos_scheduler_set_effective_priority_locked(rtos_task_t *task, uint8_t priority);
bool rtos_task_suspend(rtos_task_t *task);
bool rtos_task_resume(rtos_task_t *task);
bool rtos_scheduler_validate(void);

#endif
