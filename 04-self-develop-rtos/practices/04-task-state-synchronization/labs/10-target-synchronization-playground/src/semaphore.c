#include "semaphore.h"
#include "critical_section.h"
#include "scheduler.h"
#include "wait_list.h"

bool rtos_semaphore_init(rtos_semaphore_t *semaphore,
                       const char *name,
                       uint32_t initial_count,
                       uint32_t max_count)
{
    if ((semaphore == (rtos_semaphore_t *)0) || (name == (const char *)0) ||
        (max_count == 0U) || (initial_count > max_count)) { return false; }
    semaphore->count = initial_count;
    semaphore->max_count = max_count;
    semaphore->name = name;
    rtos_wait_list_init(&semaphore->waiters);
    return true;
}

rtos_wait_result_t rtos_semaphore_take(rtos_semaphore_t *semaphore, uint32_t timeout_ticks)
{
    rtos_task_t *task;
    const rtos_irq_state_t state = rtos_critical_enter();
    if (semaphore == (rtos_semaphore_t *)0)
    {
        rtos_critical_exit(state);
        return RTOS_WAIT_CANCELLED;
    }
    if (semaphore->count > 0U)
    {
        --semaphore->count;
        rtos_critical_exit(state);
        return RTOS_WAIT_SUCCESS;
    }
    if (timeout_ticks == RTOS_NO_WAIT)
    {
        rtos_critical_exit(state);
        return RTOS_WAIT_TIMEOUT;
    }
    task = rtos_scheduler_current();
    if (!rtos_scheduler_block_current_locked(semaphore, &semaphore->waiters,
                                           RTOS_WAIT_KIND_SEMAPHORE,
                                           timeout_ticks))
    {
        rtos_critical_exit(state);
        return RTOS_WAIT_CANCELLED;
    }
    rtos_critical_exit(state);
    return task->wait_result;
}

static bool give_locked(rtos_semaphore_t *semaphore, rtos_task_t **woken)
{
    rtos_task_t *waiter = rtos_wait_list_front(&semaphore->waiters);
    if (waiter != (rtos_task_t *)0)
    {
        if (!rtos_scheduler_wake_task_locked(waiter, RTOS_WAIT_SUCCESS)) { return false; }
        if (woken != (rtos_task_t **)0) { *woken = waiter; }
        return true;
    }
    if (semaphore->count >= semaphore->max_count) { return false; }
    ++semaphore->count;
    if (woken != (rtos_task_t **)0) { *woken = (rtos_task_t *)0; }
    return true;
}

bool rtos_semaphore_give(rtos_semaphore_t *semaphore)
{
    bool ok;
    const rtos_irq_state_t state = rtos_critical_enter();
    ok = (semaphore != (rtos_semaphore_t *)0) && give_locked(semaphore, (rtos_task_t **)0);
    rtos_critical_exit(state);
    return ok;
}

bool rtos_semaphore_give_from_isr(rtos_semaphore_t *semaphore,
                                bool *higher_priority_task_woken)
{
    rtos_task_t *woken = (rtos_task_t *)0;
    bool ok;
    const rtos_irq_state_t state = rtos_critical_enter();
    ok = (semaphore != (rtos_semaphore_t *)0) && give_locked(semaphore, &woken);
    if (higher_priority_task_woken != (bool *)0)
    {
        const rtos_task_t *current = rtos_scheduler_current();
        *higher_priority_task_woken = ok && (woken != (rtos_task_t *)0) &&
            ((current == (const rtos_task_t *)0) ||
             (woken->effective_priority < current->effective_priority));
    }
    rtos_critical_exit(state);
    return ok;
}

bool rtos_semaphore_validate(const rtos_semaphore_t *semaphore)
{
    return (semaphore != (const rtos_semaphore_t *)0) &&
           (semaphore->name != (const char *)0) &&
           (semaphore->max_count > 0U) &&
           (semaphore->count <= semaphore->max_count) &&
           rtos_wait_list_validate(&semaphore->waiters);
}
