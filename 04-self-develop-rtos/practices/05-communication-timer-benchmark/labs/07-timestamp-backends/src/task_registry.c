#include "task_registry.h"

static hr_list_t g_all_tasks;

static bool strings_equal(const char *left, const char *right)
{
    if ((left == (const char *)0) || (right == (const char *)0))
    {
        return false;
    }
    while ((*left != '\0') && (*right != '\0'))
    {
        if (*left != *right)
        {
            return false;
        }
        ++left;
        ++right;
    }
    return *left == *right;
}

void hr_task_registry_init(void)
{
    hr_list_init(&g_all_tasks);
}

bool hr_task_registry_add(hr_task_t *task)
{
    if (!hr_task_is_valid(task))
    {
        return false;
    }
    return hr_list_push_back(&g_all_tasks, &task->all_task_node);
}

size_t hr_task_registry_count(void)
{
    return hr_list_count(&g_all_tasks);
}

hr_task_t *hr_task_registry_at(size_t index)
{
    hr_list_node_t *node = hr_list_front(&g_all_tasks);
    size_t current = 0U;
    while (node != (hr_list_node_t *)0)
    {
        if (current == index)
        {
            return HR_CONTAINER_OF(node, hr_task_t, all_task_node);
        }
        node = node->next;
        ++current;
    }
    return (hr_task_t *)0;
}

hr_task_t *hr_task_registry_find(const char *name)
{
    for (size_t i = 0U; i < hr_task_registry_count(); ++i)
    {
        hr_task_t *task = hr_task_registry_at(i);
        if ((task != (hr_task_t *)0) && strings_equal(task->name, name))
        {
            return task;
        }
    }
    return (hr_task_t *)0;
}

bool hr_task_registry_validate(void)
{
    if (!hr_list_validate(&g_all_tasks))
    {
        return false;
    }
    for (size_t i = 0U; i < hr_task_registry_count(); ++i)
    {
        if (!hr_task_is_valid(hr_task_registry_at(i)))
        {
            return false;
        }
    }
    return true;
}

const hr_list_t *hr_task_registry_list(void)
{
    return &g_all_tasks;
}
