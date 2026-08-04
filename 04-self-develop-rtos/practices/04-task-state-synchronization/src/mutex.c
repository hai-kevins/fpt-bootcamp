#include "mutex.h"
#include "critical_section.h"
#include "scheduler.h"
#include "wait_list.h"

static void recompute_task_priority_locked(rtos_task_t *task)
{
    uint8_t effective;
    rtos_list_node_t *node;
    if (!rtos_task_is_valid(task))
    {
        return;
    }
    effective = task->base_priority;
    node = rtos_list_front(&task->owned_mutexes);
    while (node != (rtos_list_node_t *)0)
    {
        rtos_mutex_t *mutex = RTOS_CONTAINER_OF(node, rtos_mutex_t, owner_node);
        rtos_task_t *waiter = rtos_wait_list_front(&mutex->waiters);
        if ((waiter != (rtos_task_t *)0) && (waiter->effective_priority < effective))
        {
            effective = waiter->effective_priority;
        }
        node = node->next;
    }
    (void) rtos_scheduler_set_effective_priority_locked(task, effective);
}

bool rtos_mutex_init(rtos_mutex_t *mutex, const char *name)
{
    if ((mutex == (rtos_mutex_t *)0) || (name == (const char *)0))
    {
        return false;
    }
    mutex->name = name;
    mutex->owner = (rtos_task_t *)0;
    rtos_wait_list_init(&mutex->waiters);
    rtos_list_node_init(&mutex->owner_node);
    return true;
}

rtos_wait_result_t rtos_mutex_lock(rtos_mutex_t *mutex, uint32_t timeout_ticks)
{
    rtos_task_t *current;
    const rtos_irq_state_t state = rtos_critical_enter();
    if (mutex == (rtos_mutex_t *)0)
    {
        rtos_critical_exit(state);
        return RTOS_WAIT_CANCELLED;
    }
    current = rtos_scheduler_current();
    if (!rtos_task_is_valid(current))
    {
        rtos_critical_exit(state);
        return RTOS_WAIT_CANCELLED;
    }
    if (mutex->owner == (rtos_task_t *)0)
    {
        mutex->owner = current;
        if (!rtos_list_push_back(&current->owned_mutexes, &mutex->owner_node))
        {
            mutex->owner = (rtos_task_t *)0;
            rtos_critical_exit(state);
            return RTOS_WAIT_CANCELLED;
        }
        rtos_critical_exit(state);
        return RTOS_WAIT_SUCCESS;
    }
    if (mutex->owner == current)
    {
        rtos_critical_exit(state);
        return RTOS_WAIT_CANCELLED;
        /* Non-recursive mutex. */
    }
    if (timeout_ticks == RTOS_NO_WAIT)
    {
        rtos_critical_exit(state);
        return RTOS_WAIT_TIMEOUT;
    }
    if (current->effective_priority < mutex->owner->effective_priority)
    {
        (void) rtos_scheduler_set_effective_priority_locked(mutex->owner, current->effective_priority);
    }
    if (!rtos_scheduler_block_current_locked(mutex, &mutex->waiters, RTOS_WAIT_KIND_MUTEX, timeout_ticks))
    {
        rtos_critical_exit(state);
        return RTOS_WAIT_CANCELLED;
    }
    rtos_critical_exit(state);
    return current->wait_result;
}

bool rtos_mutex_unlock(rtos_mutex_t *mutex)
{
    rtos_task_t *old_owner;
    rtos_task_t *waiter;
    const rtos_irq_state_t state = rtos_critical_enter();
    if ((mutex == (rtos_mutex_t *)0) || (mutex->owner != rtos_scheduler_current()))
    {
        rtos_critical_exit(state);
        return false;
    }

    old_owner = mutex->owner;
    if (mutex->owner_node.owner == &old_owner->owned_mutexes)
    {
        (void) rtos_list_remove(&old_owner->owned_mutexes, &mutex->owner_node);
    }
    mutex->owner = (rtos_task_t *)0;

    waiter = rtos_wait_list_front(&mutex->waiters);
    if (waiter != (rtos_task_t *)0)
    {
        mutex->owner = waiter;
        if (!rtos_list_push_back(&waiter->owned_mutexes, &mutex->owner_node))
        {
            mutex->owner = (rtos_task_t *)0;
            rtos_critical_exit(state);
            return false;
        }
        if (!rtos_scheduler_wake_task_locked(waiter, RTOS_WAIT_SUCCESS))
        {
            rtos_critical_exit(state);
            return false;
        }
    }
    recompute_task_priority_locked(old_owner);
    rtos_critical_exit(state);
    return true;
}

void rtos_mutex_waiter_removed_locked(void *object)
{
    rtos_mutex_t *mutex = (rtos_mutex_t *) object;
    if ((mutex != (rtos_mutex_t *)0) && (mutex->owner != (rtos_task_t *)0))
    {
        recompute_task_priority_locked(mutex->owner);
    }
}

bool rtos_mutex_validate(const rtos_mutex_t *mutex)
{
    if ((mutex == (const rtos_mutex_t *)0) || (mutex->name == (const char *)0) || !rtos_wait_list_validate(&mutex->waiters))
    {
        return false;
    }
    if (mutex->owner == (const rtos_task_t *)0)
    {
        return mutex->owner_node.owner == (const void *)0;
    }
    return rtos_task_is_valid(mutex->owner) && (mutex->owner_node.owner == &mutex->owner->owned_mutexes);
}
