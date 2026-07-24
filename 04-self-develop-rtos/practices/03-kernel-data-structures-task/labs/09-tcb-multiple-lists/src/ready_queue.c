#include "ready_queue.h"

static hr_list_t g_ready_queues[HR_PRIORITY_COUNT];
static uint32_t g_ready_bitmap;

void hr_ready_init(void)
{
    for (size_t i = 0U; i < HR_PRIORITY_COUNT; ++i) { hr_list_init(&g_ready_queues[i]); }
    g_ready_bitmap = 0U;
}

bool hr_ready_insert(hr_task_t *task)
{
    const uint8_t p = (task != (hr_task_t *)0) ? task->effective_priority : 0xFFU;
    if (!hr_task_is_valid(task) || (p >= HR_PRIORITY_COUNT) ||
        (task->ready_node.owner != (const void *)0)) { return false; }
    if (!hr_list_push_back(&g_ready_queues[p], &task->ready_node)) { return false; }
    g_ready_bitmap |= (1UL << p);
    task->state = HR_TASK_READY;
    return true;
}

bool hr_ready_remove(hr_task_t *task)
{
    const uint8_t p = (task != (hr_task_t *)0) ? task->effective_priority : 0xFFU;
    if (!hr_task_is_valid(task) || (p >= HR_PRIORITY_COUNT) ||
        (task->ready_node.owner != &g_ready_queues[p])) { return false; }
    if (!hr_list_remove(&g_ready_queues[p], &task->ready_node)) { return false; }
    if (hr_list_is_empty(&g_ready_queues[p])) { g_ready_bitmap &= ~(1UL << p); }
    return true;
}

bool hr_ready_rotate(uint8_t priority)
{
    hr_list_node_t *node;
    if (priority >= HR_PRIORITY_COUNT) { return false; }
    if (hr_list_count(&g_ready_queues[priority]) <= 1U) { return true; }
    node = hr_list_pop_front(&g_ready_queues[priority]);
    return (node != (hr_list_node_t *)0) && hr_list_push_back(&g_ready_queues[priority], node);
}

hr_task_t *hr_ready_select_highest(void)
{
    for (uint8_t p = 0U; p < HR_PRIORITY_COUNT; ++p)
    {
        if ((g_ready_bitmap & (1UL << p)) != 0U)
        {
            hr_list_node_t *node = hr_list_front(&g_ready_queues[p]);
            if (node == (hr_list_node_t *)0) { return (hr_task_t *)0; }
            return HR_CONTAINER_OF(node, hr_task_t, ready_node);
        }
    }
    return (hr_task_t *)0;
}

size_t hr_ready_count(uint8_t priority)
{
    return (priority < HR_PRIORITY_COUNT) ? hr_list_count(&g_ready_queues[priority]) : 0U;
}

uint32_t hr_ready_bitmap(void) { return g_ready_bitmap; }
const hr_list_t *hr_ready_list(uint8_t priority)
{
    return (priority < HR_PRIORITY_COUNT) ? &g_ready_queues[priority] : (const hr_list_t *)0;
}

bool hr_ready_validate(void)
{
    uint32_t expected = 0U;
    for (uint8_t p = 0U; p < HR_PRIORITY_COUNT; ++p)
    {
        const hr_list_t *list = &g_ready_queues[p];
        if (!hr_list_validate(list)) { return false; }
        if (!hr_list_is_empty(list)) { expected |= (1UL << p); }
        for (hr_list_node_t *n = list->head; n != (hr_list_node_t *)0; n = n->next)
        {
            hr_task_t *task = HR_CONTAINER_OF(n, hr_task_t, ready_node);
            if (!hr_task_is_valid(task) || (task->effective_priority != p) ||
                ((task->state != HR_TASK_READY) && (task->state != HR_TASK_RUNNING))) { return false; }
        }
    }
    return expected == g_ready_bitmap;
}
