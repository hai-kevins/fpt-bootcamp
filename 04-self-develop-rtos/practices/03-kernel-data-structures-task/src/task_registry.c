#include "task_registry.h"

static rtos_list_t g_all_tasks;

static bool strings_equal(const char *left, const char *right)
{
    if ((left == (const char *)0) || (right == (const char *)0)) { return false; }
    while ((*left != '\0') && (*right != '\0'))
    {
        if (*left != *right) { return false; }
        ++left; ++right;
    }
    return *left == *right;
}

void rtos_task_registry_init(void) { rtos_list_init(&g_all_tasks); }

bool rtos_task_registry_add(rtos_task_t *task)
{
    if (!rtos_task_is_valid(task)) { return false; }
    return rtos_list_push_back(&g_all_tasks, &task->all_task_node);
}

size_t rtos_task_registry_count(void) { return rtos_list_count(&g_all_tasks); }

rtos_task_t *rtos_task_registry_at(size_t index)
{
    rtos_list_node_t *node = rtos_list_front(&g_all_tasks);
    size_t current = 0U;
    while (node != (rtos_list_node_t *)0)
    {
        if (current == index) { return RTOS_CONTAINER_OF(node, rtos_task_t, all_task_node); }
        node = node->next;
        ++current;
    }
    return (rtos_task_t *)0;
}

rtos_task_t *rtos_task_registry_find(const char *name)
{
    for (size_t i = 0U; i < rtos_task_registry_count(); ++i)
    {
        rtos_task_t *task = rtos_task_registry_at(i);
        if ((task != (rtos_task_t *)0) && strings_equal(task->name, name)) { return task; }
    }
    return (rtos_task_t *)0;
}

bool rtos_task_registry_validate(void)
{
    if (!rtos_list_validate(&g_all_tasks)) { return false; }
    for (size_t i = 0U; i < rtos_task_registry_count(); ++i)
    {
        if (!rtos_task_is_valid(rtos_task_registry_at(i))) { return false; }
    }
    return true;
}

const rtos_list_t *rtos_task_registry_list(void) { return &g_all_tasks; }
