#include "list.h"

static bool node_is_detached(const hr_list_node_t *node)
{
    return (node != (const hr_list_node_t *)0) && (node->owner == (const void *)0) && (node->previous == (hr_list_node_t *)0) && (node->next == (hr_list_node_t *)0);
}

void hr_list_init(hr_list_t *list)
{
    if (list == (hr_list_t *)0)
    {
        return;
    }
    list->head = (hr_list_node_t *)0;
    list->tail = (hr_list_node_t *)0;
    list->count = 0U;
}

void hr_list_node_init(hr_list_node_t *node)
{
    if (node == (hr_list_node_t *)0)
    {
        return;
    }
    node->previous = (hr_list_node_t *)0;
    node->next = (hr_list_node_t *)0;
    node->owner = (const void *)0;
}

bool hr_list_is_empty(const hr_list_t *list)
{
    return (list == (const hr_list_t *)0) || (list->count == 0U);
}

size_t hr_list_count(const hr_list_t *list)
{
    return (list != (const hr_list_t *)0) ? list->count : 0U;
}

hr_list_node_t *hr_list_front(const hr_list_t *list)
{
    return (list != (const hr_list_t *)0) ? list->head : (hr_list_node_t *)0;
}

hr_list_node_t *hr_list_back(const hr_list_t *list)
{
    return (list != (const hr_list_t *)0) ? list->tail : (hr_list_node_t *)0;
}

bool hr_list_push_front(hr_list_t *list, hr_list_node_t *node)
{
    if ((list == (hr_list_t *)0) || !node_is_detached(node))
    {
        return false;
    }
    node->owner = list;
    node->previous = (hr_list_node_t *)0;
    node->next = list->head;
    if (list->head != (hr_list_node_t *)0)
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

bool hr_list_push_back(hr_list_t *list, hr_list_node_t *node)
{
    if ((list == (hr_list_t *)0) || !node_is_detached(node))
    {
        return false;
    }
    node->owner = list;
    node->next = (hr_list_node_t *)0;
    node->previous = list->tail;
    if (list->tail != (hr_list_node_t *)0)
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

bool hr_list_insert_before(hr_list_t *list, hr_list_node_t *position, hr_list_node_t *node)
{
    if ((list == (hr_list_t *)0) || (position == (hr_list_node_t *)0) || (position->owner != list) || !node_is_detached(node))
    {
        return false;
    }
    if (position == list->head)
    {
        return hr_list_push_front(list, node);
    }
    node->owner = list;
    node->previous = position->previous;
    node->next = position;
    position->previous->next = node;
    position->previous = node;
    ++list->count;
    return true;
}

bool hr_list_insert_after(hr_list_t *list, hr_list_node_t *position, hr_list_node_t *node)
{
    if ((list == (hr_list_t *)0) || (position == (hr_list_node_t *)0) || (position->owner != list) || !node_is_detached(node))
    {
        return false;
    }
    if (position == list->tail)
    {
        return hr_list_push_back(list, node);
    }
    node->owner = list;
    node->next = position->next;
    node->previous = position;
    position->next->previous = node;
    position->next = node;
    ++list->count;
    return true;
}

bool hr_list_remove(hr_list_t *list, hr_list_node_t *node)
{
    if ((list == (hr_list_t *)0) || (node == (hr_list_node_t *)0) || (node->owner != list) || (list->count == 0U))
    {
        return false;
    }
    if (node->previous != (hr_list_node_t *)0)
    {
        node->previous->next = node->next;
    }
    else
    {
        list->head = node->next;
    }
    if (node->next != (hr_list_node_t *)0)
    {
        node->next->previous = node->previous;
    }
    else
    {
        list->tail = node->previous;
    }
    node->previous = (hr_list_node_t *)0;
    node->next = (hr_list_node_t *)0;
    node->owner = (const void *)0;
    --list->count;
    return true;
}

hr_list_node_t *hr_list_pop_front(hr_list_t *list)
{
    hr_list_node_t *node = hr_list_front(list);
    if (node != (hr_list_node_t *)0)
    {
        (void) hr_list_remove(list, node);
    }
    return node;
}

hr_list_node_t *hr_list_pop_back(hr_list_t *list)
{
    hr_list_node_t *node = hr_list_back(list);
    if (node != (hr_list_node_t *)0)
    {
        (void) hr_list_remove(list, node);
    }
    return node;
}

bool hr_list_validate(const hr_list_t *list)
{
    const hr_list_node_t *slow;
    const hr_list_node_t *fast;
    const hr_list_node_t *previous = (const hr_list_node_t *)0;
    const hr_list_node_t *node;
    size_t count = 0U;

    if (list == (const hr_list_t *)0)
    {
        return false;
    }
    if (list->count == 0U)
    {
        return (list->head == (hr_list_node_t *)0) && (list->tail == (hr_list_node_t *)0);
    }
    if ((list->head == (hr_list_node_t *)0) || (list->tail == (hr_list_node_t *)0) || (list->head->previous != (hr_list_node_t *)0)
        || (list->tail->next != (hr_list_node_t *)0))
    {
        return false;
    }

    slow = list->head;
    fast = list->head;
    while ((fast != (const hr_list_node_t *)0) && (fast->next != (hr_list_node_t *)0))
    {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast)
        {
            return false;
        }
    }

    node = list->head;
    while (node != (const hr_list_node_t *)0)
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
