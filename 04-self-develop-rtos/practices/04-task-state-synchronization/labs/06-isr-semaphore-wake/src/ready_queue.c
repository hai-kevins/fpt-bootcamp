#include "ready_queue.h"

static rtos_list_t g_ready_queues[RTOS_PRIORITY_COUNT];
static uint32_t g_ready_bitmap;

void rtos_ready_init(void)
{
    for (size_t i = 0U; i < RTOS_PRIORITY_COUNT; ++i) { rtos_list_init(&g_ready_queues[i]); }
    g_ready_bitmap = 0U;
}

bool rtos_ready_insert(rtos_task_t *task)
{
    const uint8_t p = (task != (rtos_task_t *)0) ? task->effective_priority : 0xFFU;
    if (!rtos_task_is_valid(task) || (p >= RTOS_PRIORITY_COUNT) ||
        (task->ready_node.owner != (const void *)0)) { return false; }
    if (!rtos_list_push_back(&g_ready_queues[p], &task->ready_node)) { return false; }
    g_ready_bitmap |= (1UL << p);
    task->state = RTOS_TASK_READY;
    return true;
}

bool rtos_ready_remove(rtos_task_t *task)
{
    const uint8_t p = (task != (rtos_task_t *)0) ? task->effective_priority : 0xFFU;
    if (!rtos_task_is_valid(task) || (p >= RTOS_PRIORITY_COUNT) ||
        (task->ready_node.owner != &g_ready_queues[p])) { return false; }
    if (!rtos_list_remove(&g_ready_queues[p], &task->ready_node)) { return false; }
    if (rtos_list_is_empty(&g_ready_queues[p])) { g_ready_bitmap &= ~(1UL << p); }
    return true;
}

bool rtos_ready_rotate(uint8_t priority)
{
    rtos_list_node_t *node;
    if (priority >= RTOS_PRIORITY_COUNT) { return false; }
    if (rtos_list_count(&g_ready_queues[priority]) <= 1U) { return true; }
    node = rtos_list_pop_front(&g_ready_queues[priority]);
    return (node != (rtos_list_node_t *)0) && rtos_list_push_back(&g_ready_queues[priority], node);
}

rtos_task_t *rtos_ready_select_highest(void)
{
    for (uint8_t p = 0U; p < RTOS_PRIORITY_COUNT; ++p)
    {
        if ((g_ready_bitmap & (1UL << p)) != 0U)
        {
            rtos_list_node_t *node = rtos_list_front(&g_ready_queues[p]);
            if (node == (rtos_list_node_t *)0) { return (rtos_task_t *)0; }
            return RTOS_CONTAINER_OF(node, rtos_task_t, ready_node);
        }
    }
    return (rtos_task_t *)0;
}

size_t rtos_ready_count(uint8_t priority)
{
    return (priority < RTOS_PRIORITY_COUNT) ? rtos_list_count(&g_ready_queues[priority]) : 0U;
}

uint32_t rtos_ready_bitmap(void) { return g_ready_bitmap; }
const rtos_list_t *rtos_ready_list(uint8_t priority)
{
    return (priority < RTOS_PRIORITY_COUNT) ? &g_ready_queues[priority] : (const rtos_list_t *)0;
}

bool rtos_ready_validate(void)
{
    uint32_t expected = 0U;
    for (uint8_t p = 0U; p < RTOS_PRIORITY_COUNT; ++p)
    {
        const rtos_list_t *list = &g_ready_queues[p];
        if (!rtos_list_validate(list)) { return false; }
        if (!rtos_list_is_empty(list)) { expected |= (1UL << p); }
        for (rtos_list_node_t *n = list->head; n != (rtos_list_node_t *)0; n = n->next)
        {
            rtos_task_t *task = RTOS_CONTAINER_OF(n, rtos_task_t, ready_node);
            if (!rtos_task_is_valid(task) || (task->effective_priority != p) ||
                ((task->state != RTOS_TASK_READY) && (task->state != RTOS_TASK_RUNNING))) { return false; }
        }
    }
    return expected == g_ready_bitmap;
}
