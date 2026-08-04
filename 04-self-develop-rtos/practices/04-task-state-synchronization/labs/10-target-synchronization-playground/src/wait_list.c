#include "wait_list.h"

void rtos_wait_list_init(rtos_list_t *list)
{
    rtos_list_init(list);
}

bool rtos_wait_list_insert(rtos_list_t *list, rtos_task_t *task)
{
    rtos_list_node_t *node;
    if ((list == (rtos_list_t *)0) || !rtos_task_is_valid(task) || (task->wait_node.owner != (const void *)0))
    {
        return false;
    }

    node = rtos_list_front(list);
    while (node != (rtos_list_node_t *)0)
    {
        rtos_task_t *other = RTOS_CONTAINER_OF(node, rtos_task_t, wait_node);
        if (task->effective_priority < other->effective_priority)
        {
            return rtos_list_insert_before(list, node, &task->wait_node);
        }
        node = node->next;
    }
    return rtos_list_push_back(list, &task->wait_node);
}

bool rtos_wait_list_remove(rtos_list_t *list, rtos_task_t *task)
{
    return (list != (rtos_list_t *)0) && rtos_task_is_valid(task) && rtos_list_remove(list, &task->wait_node);
}

rtos_task_t *rtos_wait_list_front(const rtos_list_t *list)
{
    rtos_list_node_t *node = rtos_list_front(list);
    return (node != (rtos_list_node_t *)0) ? RTOS_CONTAINER_OF(node, rtos_task_t, wait_node) : (rtos_task_t *)0;
}

bool rtos_wait_list_validate(const rtos_list_t *list)
{
    const rtos_list_node_t *node;
    uint8_t previous_priority = 0U;
    bool first = true;
    if (!rtos_list_validate(list))
    {
        return false;
    }
    node = list->head;
    while (node != (const rtos_list_node_t *)0)
    {
        const rtos_task_t *task = RTOS_CONTAINER_OF(node, rtos_task_t, wait_node);
        if (!rtos_task_is_valid(task) || (task->state != RTOS_TASK_BLOCKED))
        {
            return false;
        }
        if (!first && (task->effective_priority < previous_priority))
        {
            return false;
        }
        previous_priority = task->effective_priority;
        first = false;
        node = node->next;
    }
    return true;
}
