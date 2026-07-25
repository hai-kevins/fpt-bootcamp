#include "list.h"

static bool node_is_detached(const rtos_list_node_t *node)
{
    return (node != (const rtos_list_node_t *)0) &&
           (node->owner == (const void *)0) &&
           (node->previous == (rtos_list_node_t *)0) &&
           (node->next == (rtos_list_node_t *)0);
}

void rtos_list_init(rtos_list_t *list)
{
    if (list == (rtos_list_t *)0) { return; }
    list->head = (rtos_list_node_t *)0;
    list->tail = (rtos_list_node_t *)0;
    list->count = 0U;
}

void rtos_list_node_init(rtos_list_node_t *node)
{
    if (node == (rtos_list_node_t *)0) { return; }
    node->previous = (rtos_list_node_t *)0;
    node->next = (rtos_list_node_t *)0;
    node->owner = (const void *)0;
}

bool rtos_list_is_empty(const rtos_list_t *list)
{
    return (list == (const rtos_list_t *)0) || (list->count == 0U);
}

size_t rtos_list_count(const rtos_list_t *list)
{
    return (list != (const rtos_list_t *)0) ? list->count : 0U;
}

rtos_list_node_t *rtos_list_front(const rtos_list_t *list)
{
    return (list != (const rtos_list_t *)0) ? list->head : (rtos_list_node_t *)0;
}

rtos_list_node_t *rtos_list_back(const rtos_list_t *list)
{
    return (list != (const rtos_list_t *)0) ? list->tail : (rtos_list_node_t *)0;
}

bool rtos_list_push_front(rtos_list_t *list, rtos_list_node_t *node)
{
    if ((list == (rtos_list_t *)0) || !node_is_detached(node)) { return false; }
    node->owner = list;
    node->previous = (rtos_list_node_t *)0;
    node->next = list->head;
    if (list->head != (rtos_list_node_t *)0) { list->head->previous = node; }
    else { list->tail = node; }
    list->head = node;
    ++list->count;
    return true;
}

bool rtos_list_push_back(rtos_list_t *list, rtos_list_node_t *node)
{
    if ((list == (rtos_list_t *)0) || !node_is_detached(node)) { return false; }
    node->owner = list;
    node->next = (rtos_list_node_t *)0;
    node->previous = list->tail;
    if (list->tail != (rtos_list_node_t *)0) { list->tail->next = node; }
    else { list->head = node; }
    list->tail = node;
    ++list->count;
    return true;
}

bool rtos_list_insert_before(rtos_list_t *list, rtos_list_node_t *position, rtos_list_node_t *node)
{
    if ((list == (rtos_list_t *)0) || (position == (rtos_list_node_t *)0) ||
        (position->owner != list) || !node_is_detached(node)) { return false; }
    if (position == list->head) { return rtos_list_push_front(list, node); }
    node->owner = list;
    node->previous = position->previous;
    node->next = position;
    position->previous->next = node;
    position->previous = node;
    ++list->count;
    return true;
}

bool rtos_list_insert_after(rtos_list_t *list, rtos_list_node_t *position, rtos_list_node_t *node)
{
    if ((list == (rtos_list_t *)0) || (position == (rtos_list_node_t *)0) ||
        (position->owner != list) || !node_is_detached(node)) { return false; }
    if (position == list->tail) { return rtos_list_push_back(list, node); }
    node->owner = list;
    node->next = position->next;
    node->previous = position;
    position->next->previous = node;
    position->next = node;
    ++list->count;
    return true;
}

bool rtos_list_remove(rtos_list_t *list, rtos_list_node_t *node)
{
    if ((list == (rtos_list_t *)0) || (node == (rtos_list_node_t *)0) ||
        (node->owner != list) || (list->count == 0U)) { return false; }
    if (node->previous != (rtos_list_node_t *)0) { node->previous->next = node->next; }
    else { list->head = node->next; }
    if (node->next != (rtos_list_node_t *)0) { node->next->previous = node->previous; }
    else { list->tail = node->previous; }
    node->previous = (rtos_list_node_t *)0;
    node->next = (rtos_list_node_t *)0;
    node->owner = (const void *)0;
    --list->count;
    return true;
}

rtos_list_node_t *rtos_list_pop_front(rtos_list_t *list)
{
    rtos_list_node_t *node = rtos_list_front(list);
    if (node != (rtos_list_node_t *)0) { (void)rtos_list_remove(list, node); }
    return node;
}

rtos_list_node_t *rtos_list_pop_back(rtos_list_t *list)
{
    rtos_list_node_t *node = rtos_list_back(list);
    if (node != (rtos_list_node_t *)0) { (void)rtos_list_remove(list, node); }
    return node;
}

bool rtos_list_validate(const rtos_list_t *list)
{
    const rtos_list_node_t *slow;
    const rtos_list_node_t *fast;
    const rtos_list_node_t *previous = (const rtos_list_node_t *)0;
    const rtos_list_node_t *node;
    size_t count = 0U;

    if (list == (const rtos_list_t *)0) { return false; }
    if (list->count == 0U)
    {
        return (list->head == (rtos_list_node_t *)0) &&
               (list->tail == (rtos_list_node_t *)0);
    }
    if ((list->head == (rtos_list_node_t *)0) || (list->tail == (rtos_list_node_t *)0) ||
        (list->head->previous != (rtos_list_node_t *)0) ||
        (list->tail->next != (rtos_list_node_t *)0)) { return false; }

    slow = list->head;
    fast = list->head;
    while ((fast != (const rtos_list_node_t *)0) &&
           (fast->next != (rtos_list_node_t *)0))
    {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) { return false; }
    }

    node = list->head;
    while (node != (const rtos_list_node_t *)0)
    {
        if ((node->owner != list) || (node->previous != previous)) { return false; }
        previous = node;
        node = node->next;
        ++count;
        if (count > list->count) { return false; }
    }
    return (previous == list->tail) && (count == list->count);
}
