#include "timeout.h"

static rtos_list_t g_delayed_tasks;

void rtos_timeout_init(void)
{
    rtos_list_init(&g_delayed_tasks);
}

bool rtos_tick_reached(uint32_t now, uint32_t deadline)
{
    return (int32_t)(now - deadline) >= 0;
}

bool rtos_tick_before(uint32_t left, uint32_t right)
{
    return (int32_t)(left - right) < 0;
}

bool rtos_timeout_insert(rtos_task_t *task)
{
    rtos_list_node_t *node;
    if (!rtos_task_is_valid(task) || (task->wake_tick == RTOS_WAIT_FOREVER) || (task->timeout_node.owner != (const void *)0))
    {
        return false;
    }

    node = rtos_list_front(&g_delayed_tasks);
    while (node != (rtos_list_node_t *)0)
    {
        rtos_task_t *other = RTOS_CONTAINER_OF(node, rtos_task_t, timeout_node);
        if (rtos_tick_before(task->wake_tick, other->wake_tick))
        {
            return rtos_list_insert_before(&g_delayed_tasks, node, &task->timeout_node);
        }
        node = node->next;
    }
    return rtos_list_push_back(&g_delayed_tasks, &task->timeout_node);
}

bool rtos_timeout_remove(rtos_task_t *task)
{
    return rtos_task_is_valid(task) && (task->timeout_node.owner == &g_delayed_tasks) && rtos_list_remove(&g_delayed_tasks,
        &task->timeout_node);
}

rtos_task_t *rtos_timeout_front(void)
{
    rtos_list_node_t *node = rtos_list_front(&g_delayed_tasks);
    return (node != (rtos_list_node_t *)0) ? RTOS_CONTAINER_OF(node, rtos_task_t, timeout_node) : (rtos_task_t *)0;
}

rtos_task_t *rtos_timeout_take_due(uint32_t now)
{
    rtos_task_t *task = rtos_timeout_front();
    if ((task == (rtos_task_t *)0) || !rtos_tick_reached(now, task->wake_tick))
    {
        return (rtos_task_t *)0;
    }
    (void) rtos_timeout_remove(task);
    return task;
}

size_t rtos_timeout_count(void)
{
    return rtos_list_count(&g_delayed_tasks);
}

const rtos_list_t *rtos_timeout_list(void)
{
    return &g_delayed_tasks;
}

bool rtos_timeout_validate(void)
{
    const rtos_list_node_t *node;
    const rtos_task_t *previous = (const rtos_task_t *)0;
    if (!rtos_list_validate(&g_delayed_tasks))
    {
        return false;
    }
    node = g_delayed_tasks.head;
    while (node != (const rtos_list_node_t *)0)
    {
        const rtos_task_t *task = RTOS_CONTAINER_OF(node, rtos_task_t, timeout_node);
        if (!rtos_task_is_valid(task) || (task->state != RTOS_TASK_BLOCKED) || (task->wake_tick == RTOS_WAIT_FOREVER))
        {
            return false;
        }
        if ((previous != (const rtos_task_t *)0) && rtos_tick_before(task->wake_tick, previous->wake_tick))
        {
            return false;
        }
        previous = task;
        node = node->next;
    }
    return true;
}
