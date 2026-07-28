#include "wait_list.h"

void hr_wait_list_init(hr_list_t *list) { hr_list_init(list); }

bool hr_wait_list_insert(hr_list_t *list, hr_task_t *task)
{
    hr_list_node_t *node;
    if ((list == (hr_list_t *)0) || !hr_task_is_valid(task) ||
        (task->wait_node.owner != (const void *)0)) { return false; }

    node = hr_list_front(list);
    while (node != (hr_list_node_t *)0)
    {
        hr_task_t *other = HR_CONTAINER_OF(node, hr_task_t, wait_node);
        if (task->effective_priority < other->effective_priority)
        {
            return hr_list_insert_before(list, node, &task->wait_node);
        }
        node = node->next;
    }
    return hr_list_push_back(list, &task->wait_node);
}

bool hr_wait_list_remove(hr_list_t *list, hr_task_t *task)
{
    return (list != (hr_list_t *)0) && hr_task_is_valid(task) &&
           hr_list_remove(list, &task->wait_node);
}

hr_task_t *hr_wait_list_front(const hr_list_t *list)
{
    hr_list_node_t *node = hr_list_front(list);
    return (node != (hr_list_node_t *)0) ?
           HR_CONTAINER_OF(node, hr_task_t, wait_node) : (hr_task_t *)0;
}

bool hr_wait_list_validate(const hr_list_t *list)
{
    const hr_list_node_t *node;
    uint8_t previous_priority = 0U;
    bool first = true;
    if (!hr_list_validate(list)) { return false; }
    node = list->head;
    while (node != (const hr_list_node_t *)0)
    {
        const hr_task_t *task = HR_CONTAINER_OF(node, hr_task_t, wait_node);
        if (!hr_task_is_valid(task) || (task->state != HR_TASK_BLOCKED)) { return false; }
        if (!first && (task->effective_priority < previous_priority)) { return false; }
        previous_priority = task->effective_priority;
        first = false;
        node = node->next;
    }
    return true;
}
