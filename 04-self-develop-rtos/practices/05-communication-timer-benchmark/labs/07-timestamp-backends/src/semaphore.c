#include "semaphore.h"
#include "critical_section.h"
#include "scheduler.h"
#include "wait_list.h"

bool hr_semaphore_init(hr_semaphore_t *semaphore, const char *name, uint32_t initial_count, uint32_t max_count)
{
    if ((semaphore == (hr_semaphore_t *)0) || (name == (const char *)0) || (max_count == 0U) || (initial_count > max_count))
    {
        return false;
    }
    semaphore->count = initial_count;
    semaphore->max_count = max_count;
    semaphore->name = name;
    hr_wait_list_init(&semaphore->waiters);
    return true;
}

hr_wait_result_t hr_semaphore_take(hr_semaphore_t *semaphore, uint32_t timeout_ticks)
{
    hr_task_t *task;
    const hr_irq_state_t state = hr_critical_enter();
    if (semaphore == (hr_semaphore_t *)0)
    {
        hr_critical_exit(state);
        return HR_WAIT_CANCELLED;
    }
    if (semaphore->count > 0U)
    {
        --semaphore->count;
        hr_critical_exit(state);
        return HR_WAIT_SUCCESS;
    }
    if (timeout_ticks == HR_NO_WAIT)
    {
        hr_critical_exit(state);
        return HR_WAIT_TIMEOUT;
    }
    task = hr_scheduler_current();
    if (!hr_scheduler_block_current_locked(semaphore, &semaphore->waiters, HR_WAIT_KIND_SEMAPHORE, timeout_ticks))
    {
        hr_critical_exit(state);
        return HR_WAIT_CANCELLED;
    }
    hr_critical_exit(state);
    return task->wait_result;
}

static bool give_locked(hr_semaphore_t *semaphore, hr_task_t * *woken)
{
    hr_task_t *waiter = hr_wait_list_front(&semaphore->waiters);
    if (waiter != (hr_task_t *)0)
    {
        if (!hr_scheduler_wake_task_locked(waiter, HR_WAIT_SUCCESS))
        {
            return false;
        }
        if (woken != (hr_task_t * *)0)
        {
            *woken = waiter;
        }
        return true;
    }
    if (semaphore->count >= semaphore->max_count)
    {
        return false;
    }
    ++semaphore->count;
    if (woken != (hr_task_t * *)0)
    {
        *woken = (hr_task_t *)0;
    }
    return true;
}

bool hr_semaphore_give(hr_semaphore_t *semaphore)
{
    bool ok;
    const hr_irq_state_t state = hr_critical_enter();
    ok = (semaphore != (hr_semaphore_t *)0) && give_locked(semaphore, (hr_task_t * *)0);
    hr_critical_exit(state);
    return ok;
}

bool hr_semaphore_give_from_isr(hr_semaphore_t *semaphore, bool *higher_priority_task_woken)
{
    hr_task_t *woken = (hr_task_t *)0;
    bool ok;
    const hr_irq_state_t state = hr_critical_enter();
    ok = (semaphore != (hr_semaphore_t *)0) && give_locked(semaphore, &woken);
    if (higher_priority_task_woken != (bool *)0)
    {
        const hr_task_t *current = hr_scheduler_current();
        *higher_priority_task_woken = ok && (woken != (hr_task_t *)0) && ((current == (const hr_task_t *)0)
            || (woken->effective_priority < current->effective_priority));
    }
    hr_critical_exit(state);
    return ok;
}

bool hr_semaphore_validate(const hr_semaphore_t *semaphore)
{
    return (semaphore != (const hr_semaphore_t *)0) && (semaphore->name != (const char *)0) && (semaphore->max_count > 0U) && (semaphore->count <= semaphore->max_count) && hr_wait_list_validate(&semaphore->waiters);
}
