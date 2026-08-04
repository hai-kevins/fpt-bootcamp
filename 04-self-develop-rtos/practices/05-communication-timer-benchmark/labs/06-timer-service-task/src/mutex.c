#include "mutex.h"
#include "critical_section.h"
#include "scheduler.h"
#include "wait_list.h"

static void recompute_task_priority_locked(hr_task_t *task)
{
    uint8_t effective;
    hr_list_node_t *node;
    if (!hr_task_is_valid(task))
    {
        return;
    }
    effective = task->base_priority;
    node = hr_list_front(&task->owned_mutexes);
    while (node != (hr_list_node_t *)0)
    {
        hr_mutex_t *mutex = HR_CONTAINER_OF(node, hr_mutex_t, owner_node);
        hr_task_t *waiter = hr_wait_list_front(&mutex->waiters);
        if ((waiter != (hr_task_t *)0) && (waiter->effective_priority < effective))
        {
            effective = waiter->effective_priority;
        }
        node = node->next;
    }
    (void) hr_scheduler_set_effective_priority_locked(task, effective);
}

bool hr_mutex_init(hr_mutex_t *mutex, const char *name)
{
    if ((mutex == (hr_mutex_t *)0) || (name == (const char *)0))
    {
        return false;
    }
    mutex->name = name;
    mutex->owner = (hr_task_t *)0;
    hr_wait_list_init(&mutex->waiters);
    hr_list_node_init(&mutex->owner_node);
    return true;
}

hr_wait_result_t hr_mutex_lock(hr_mutex_t *mutex, uint32_t timeout_ticks)
{
    hr_task_t *current;
    const hr_irq_state_t state = hr_critical_enter();
    if (mutex == (hr_mutex_t *)0)
    {
        hr_critical_exit(state);
        return HR_WAIT_CANCELLED;
    }
    current = hr_scheduler_current();
    if (!hr_task_is_valid(current))
    {
        hr_critical_exit(state);
        return HR_WAIT_CANCELLED;
    }
    if (mutex->owner == (hr_task_t *)0)
    {
        mutex->owner = current;
        if (!hr_list_push_back(&current->owned_mutexes, &mutex->owner_node))
        {
            mutex->owner = (hr_task_t *)0;
            hr_critical_exit(state);
            return HR_WAIT_CANCELLED;
        }
        hr_critical_exit(state);
        return HR_WAIT_SUCCESS;
    }
    if (mutex->owner == current)
    {
        hr_critical_exit(state);
        return HR_WAIT_CANCELLED;
        /* Non-recursive mutex. */
    }
    if (timeout_ticks == HR_NO_WAIT)
    {
        hr_critical_exit(state);
        return HR_WAIT_TIMEOUT;
    }
    if (current->effective_priority < mutex->owner->effective_priority)
    {
        (void) hr_scheduler_set_effective_priority_locked(mutex->owner, current->effective_priority);
    }
    if (!hr_scheduler_block_current_locked(mutex, &mutex->waiters, HR_WAIT_KIND_MUTEX, timeout_ticks))
    {
        hr_critical_exit(state);
        return HR_WAIT_CANCELLED;
    }
    hr_critical_exit(state);
    return current->wait_result;
}

bool hr_mutex_unlock(hr_mutex_t *mutex)
{
    hr_task_t *old_owner;
    hr_task_t *waiter;
    const hr_irq_state_t state = hr_critical_enter();
    if ((mutex == (hr_mutex_t *)0) || (mutex->owner != hr_scheduler_current()))
    {
        hr_critical_exit(state);
        return false;
    }

    old_owner = mutex->owner;
    if (mutex->owner_node.owner == &old_owner->owned_mutexes)
    {
        (void) hr_list_remove(&old_owner->owned_mutexes, &mutex->owner_node);
    }
    mutex->owner = (hr_task_t *)0;

    waiter = hr_wait_list_front(&mutex->waiters);
    if (waiter != (hr_task_t *)0)
    {
        mutex->owner = waiter;
        if (!hr_list_push_back(&waiter->owned_mutexes, &mutex->owner_node))
        {
            mutex->owner = (hr_task_t *)0;
            hr_critical_exit(state);
            return false;
        }
        if (!hr_scheduler_wake_task_locked(waiter, HR_WAIT_SUCCESS))
        {
            hr_critical_exit(state);
            return false;
        }
    }
    recompute_task_priority_locked(old_owner);
    hr_critical_exit(state);
    return true;
}

void hr_mutex_waiter_removed_locked(void *object)
{
    hr_mutex_t *mutex = (hr_mutex_t *) object;
    if ((mutex != (hr_mutex_t *)0) && (mutex->owner != (hr_task_t *)0))
    {
        recompute_task_priority_locked(mutex->owner);
    }
}

bool hr_mutex_validate(const hr_mutex_t *mutex)
{
    if ((mutex == (const hr_mutex_t *)0) || (mutex->name == (const char *)0) || !hr_wait_list_validate(&mutex->waiters))
    {
        return false;
    }
    if (mutex->owner == (const hr_task_t *)0)
    {
        return mutex->owner_node.owner == (const void *)0;
    }
    return hr_task_is_valid(mutex->owner) && (mutex->owner_node.owner == &mutex->owner->owned_mutexes);
}
