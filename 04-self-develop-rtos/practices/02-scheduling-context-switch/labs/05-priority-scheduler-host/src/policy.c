#include "policy.h"

void policy_init(policy_scheduler_t *scheduler)
{
    if (scheduler == NULL)
    {
        return;
    }

    for (size_t index = 0U; index < POLICY_PRIORITIES; ++index)
    {
        scheduler->head[index] = NULL;
        scheduler->tail[index] = NULL;
        scheduler->count[index] = 0U;
    }
    scheduler->bitmap = 0U;
}

bool policy_add(policy_scheduler_t *scheduler, policy_task_t *task)
{
    const uint8_t priority = (task != NULL) ? task->priority : 0U;

    if ((scheduler == NULL) || (task == NULL) || task->queued || (priority >= POLICY_PRIORITIES))
    {
        return false;
    }

    task->next = NULL;
    if (scheduler->tail[priority] != NULL)
    {
        scheduler->tail[priority]->next = task;
    }
    else
    {
        scheduler->head[priority] = task;
    }
    scheduler->tail[priority] = task;
    ++scheduler->count[priority];
    scheduler->bitmap |= (1UL << priority);
    task->queued = true;
    return true;
}

policy_task_t *policy_select(const policy_scheduler_t *scheduler)
{
    if (scheduler == NULL)
    {
        return NULL;
    }

    for (uint8_t priority = 0U; priority < POLICY_PRIORITIES; ++priority)
    {
        if ((scheduler->bitmap & (1UL << priority)) != 0U)
        {
            return scheduler->head[priority];
        }
    }
    return NULL;
}

bool policy_rotate(policy_scheduler_t *scheduler, uint8_t priority)
{
    policy_task_t *first;

    if ((scheduler == NULL) || (priority >= POLICY_PRIORITIES) || (scheduler->count[priority] <= 1U))
    {
        return false;
    }

    first = scheduler->head[priority];
    scheduler->head[priority] = first->next;
    first->next = NULL;
    scheduler->tail[priority]->next = first;
    scheduler->tail[priority] = first;
    return true;
}

bool policy_remove(policy_scheduler_t *scheduler, policy_task_t *task)
{
    policy_task_t *previous = NULL;
    policy_task_t *cursor;
    const uint8_t priority = (task != NULL) ? task->priority : 0U;

    if ((scheduler == NULL) || (task == NULL) || !task->queued || (priority >= POLICY_PRIORITIES))
    {
        return false;
    }

    cursor = scheduler->head[priority];
    while ((cursor != NULL) && (cursor != task))
    {
        previous = cursor;
        cursor = cursor->next;
    }

    if (cursor == NULL)
    {
        return false;
    }

    if (previous != NULL)
    {
        previous->next = cursor->next;
    }
    else
    {
        scheduler->head[priority] = cursor->next;
    }

    if (scheduler->tail[priority] == cursor)
    {
        scheduler->tail[priority] = previous;
    }

    cursor->next = NULL;
    cursor->queued = false;
    --scheduler->count[priority];
    if (scheduler->count[priority] == 0U)
    {
        scheduler->bitmap &= ~ (1UL << priority);
    }
    return true;
}
