#include "dlist.h"

static bool node_is_detached(const dlist_node_t *node)
{
    return (node != (const dlist_node_t *)0) && (node->owner == (const void *)0) && (node->previous == (dlist_node_t *)0) && (node->next == (dlist_node_t *)0);
}

void dlist_init(dlist_t *list)
{
    if (list == (dlist_t *)0)
    {
        return;
    }
    list->head = (dlist_node_t *)0;
    list->tail = (dlist_node_t *)0;
    list->count = 0U;
}

void dlist_node_init(dlist_node_t *node)
{
    if (node == (dlist_node_t *)0)
    {
        return;
    }
    node->previous = (dlist_node_t *)0;
    node->next = (dlist_node_t *)0;
    node->owner = (const void *)0;
}

bool dlist_is_empty(const dlist_t *list)
{
    return (list == (const dlist_t *)0) || (list->count == 0U);
}

size_t dlist_count(const dlist_t *list)
{
    return (list != (const dlist_t *)0) ? list->count : 0U;
}

dlist_node_t *dlist_front(const dlist_t *list)
{
    return (list != (const dlist_t *)0) ? list->head : (dlist_node_t *)0;
}

dlist_node_t *dlist_back(const dlist_t *list)
{
    return (list != (const dlist_t *)0) ? list->tail : (dlist_node_t *)0;
}

bool dlist_push_front(dlist_t *list, dlist_node_t *node)
{
    if ((list == (dlist_t *)0) || !node_is_detached(node))
    {
        return false;
    }
    node->owner = list;
    node->previous = (dlist_node_t *)0;
    node->next = list->head;
    if (list->head != (dlist_node_t *)0)
    {
        list->head->previous = node;
    }
    else
    {
        list->tail = node;
    }
    list->head = node;
    ++list->count;
    return true;
}

bool dlist_push_back(dlist_t *list, dlist_node_t *node)
{
    if ((list == (dlist_t *)0) || !node_is_detached(node))
    {
        return false;
    }
    node->owner = list;
    node->next = (dlist_node_t *)0;
    node->previous = list->tail;
    if (list->tail != (dlist_node_t *)0)
    {
        list->tail->next = node;
    }
    else
    {
        list->head = node;
    }
    list->tail = node;
    ++list->count;
    return true;
}

bool dlist_insert_before(dlist_t *list, dlist_node_t *position, dlist_node_t *node)
{
    if ((list == (dlist_t *)0) || (position == (dlist_node_t *)0) || (position->owner != list) || !node_is_detached(node))
    {
        return false;
    }
    if (position == list->head)
    {
        return dlist_push_front(list, node);
    }
    node->owner = list;
    node->previous = position->previous;
    node->next = position;
    position->previous->next = node;
    position->previous = node;
    ++list->count;
    return true;
}

bool dlist_insert_after(dlist_t *list, dlist_node_t *position, dlist_node_t *node)
{
    if ((list == (dlist_t *)0) || (position == (dlist_node_t *)0) || (position->owner != list) || !node_is_detached(node))
    {
        return false;
    }
    if (position == list->tail)
    {
        return dlist_push_back(list, node);
    }
    node->owner = list;
    node->next = position->next;
    node->previous = position;
    position->next->previous = node;
    position->next = node;
    ++list->count;
    return true;
}

bool dlist_remove(dlist_t *list, dlist_node_t *node)
{
    if ((list == (dlist_t *)0) || (node == (dlist_node_t *)0) || (node->owner != list) || (list->count == 0U))
    {
        return false;
    }
    if (node->previous != (dlist_node_t *)0)
    {
        node->previous->next = node->next;
    }
    else
    {
        list->head = node->next;
    }
    if (node->next != (dlist_node_t *)0)
    {
        node->next->previous = node->previous;
    }
    else
    {
        list->tail = node->previous;
    }
    node->previous = (dlist_node_t *)0;
    node->next = (dlist_node_t *)0;
    node->owner = (const void *)0;
    --list->count;
    return true;
}

dlist_node_t *dlist_pop_front(dlist_t *list)
{
    dlist_node_t *node = dlist_front(list);
    if (node != (dlist_node_t *)0)
    {
        (void) dlist_remove(list, node);
    }
    return node;
}

dlist_node_t *dlist_pop_back(dlist_t *list)
{
    dlist_node_t *node = dlist_back(list);
    if (node != (dlist_node_t *)0)
    {
        (void) dlist_remove(list, node);
    }
    return node;
}

bool dlist_validate(const dlist_t *list)
{
    const dlist_node_t *slow;
    const dlist_node_t *fast;
    const dlist_node_t *previous = (const dlist_node_t *)0;
    const dlist_node_t *node;
    size_t count = 0U;

    if (list == (const dlist_t *)0)
    {
        return false;
    }
    if (list->count == 0U)
    {
        return (list->head == (dlist_node_t *)0) && (list->tail == (dlist_node_t *)0);
    }
    if ((list->head == (dlist_node_t *)0) || (list->tail == (dlist_node_t *)0) || (list->head->previous != (dlist_node_t *)0)
        || (list->tail->next != (dlist_node_t *)0))
    {
        return false;
    }

    slow = list->head;
    fast = list->head;
    while ((fast != (const dlist_node_t *)0) && (fast->next != (dlist_node_t *)0))
    {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast)
        {
            return false;
        }
    }

    node = list->head;
    while (node != (const dlist_node_t *)0)
    {
        if ((node->owner != list) || (node->previous != previous))
        {
            return false;
        }
        previous = node;
        node = node->next;
        ++count;
        if (count > list->count)
        {
            return false;
        }
    }
    return (previous == list->tail) && (count == list->count);
}
