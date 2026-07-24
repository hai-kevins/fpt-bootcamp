#include "timeout.h"

static hr_list_t g_delayed_tasks;

void hr_timeout_init(void) { hr_list_init(&g_delayed_tasks); }

bool hr_tick_reached(uint32_t now, uint32_t deadline)
{
    return (int32_t)(now - deadline) >= 0;
}

bool hr_tick_before(uint32_t left, uint32_t right)
{
    return (int32_t)(left - right) < 0;
}

bool hr_timeout_insert(hr_task_t *task)
{
    hr_list_node_t *node;
    if (!hr_task_is_valid(task) || (task->wake_tick == HR_WAIT_FOREVER) ||
        (task->timeout_node.owner != (const void *)0)) { return false; }

    node = hr_list_front(&g_delayed_tasks);
    while (node != (hr_list_node_t *)0)
    {
        hr_task_t *other = HR_CONTAINER_OF(node, hr_task_t, timeout_node);
        if (hr_tick_before(task->wake_tick, other->wake_tick))
        {
            return hr_list_insert_before(&g_delayed_tasks, node, &task->timeout_node);
        }
        node = node->next;
    }
    return hr_list_push_back(&g_delayed_tasks, &task->timeout_node);
}

bool hr_timeout_remove(hr_task_t *task)
{
    return hr_task_is_valid(task) &&
           (task->timeout_node.owner == &g_delayed_tasks) &&
           hr_list_remove(&g_delayed_tasks, &task->timeout_node);
}

hr_task_t *hr_timeout_front(void)
{
    hr_list_node_t *node = hr_list_front(&g_delayed_tasks);
    return (node != (hr_list_node_t *)0) ?
           HR_CONTAINER_OF(node, hr_task_t, timeout_node) : (hr_task_t *)0;
}

hr_task_t *hr_timeout_take_due(uint32_t now)
{
    hr_task_t *task = hr_timeout_front();
    if ((task == (hr_task_t *)0) || !hr_tick_reached(now, task->wake_tick))
    {
        return (hr_task_t *)0;
    }
    (void)hr_timeout_remove(task);
    return task;
}

size_t hr_timeout_count(void) { return hr_list_count(&g_delayed_tasks); }
const hr_list_t *hr_timeout_list(void) { return &g_delayed_tasks; }

bool hr_timeout_validate(void)
{
    const hr_list_node_t *node;
    const hr_task_t *previous = (const hr_task_t *)0;
    if (!hr_list_validate(&g_delayed_tasks)) { return false; }
    node = g_delayed_tasks.head;
    while (node != (const hr_list_node_t *)0)
    {
        const hr_task_t *task = HR_CONTAINER_OF(node, hr_task_t, timeout_node);
        if (!hr_task_is_valid(task) || (task->state != HR_TASK_BLOCKED) ||
            (task->wake_tick == HR_WAIT_FOREVER)) { return false; }
        if ((previous != (const hr_task_t *)0) &&
            hr_tick_before(task->wake_tick, previous->wake_tick)) { return false; }
        previous = task;
        node = node->next;
    }
    return true;
}
