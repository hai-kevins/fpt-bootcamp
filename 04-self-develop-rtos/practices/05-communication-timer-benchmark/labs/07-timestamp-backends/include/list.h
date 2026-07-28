#ifndef HR_LIST_H
#define HR_LIST_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct hr_list_node
{
    struct hr_list_node *previous;
    struct hr_list_node *next;
    const void *owner;
} hr_list_node_t;

typedef struct
{
    hr_list_node_t *head;
    hr_list_node_t *tail;
    size_t count;
} hr_list_t;

#define HR_CONTAINER_OF(pointer, type, member) \
    ((type *)((uint8_t *)(pointer) - offsetof(type, member)))

void hr_list_init(hr_list_t *list);
void hr_list_node_init(hr_list_node_t *node);
bool hr_list_is_empty(const hr_list_t *list);
size_t hr_list_count(const hr_list_t *list);
hr_list_node_t *hr_list_front(const hr_list_t *list);
hr_list_node_t *hr_list_back(const hr_list_t *list);
bool hr_list_push_front(hr_list_t *list, hr_list_node_t *node);
bool hr_list_push_back(hr_list_t *list, hr_list_node_t *node);
bool hr_list_insert_before(hr_list_t *list, hr_list_node_t *position, hr_list_node_t *node);
bool hr_list_insert_after(hr_list_t *list, hr_list_node_t *position, hr_list_node_t *node);
hr_list_node_t *hr_list_pop_front(hr_list_t *list);
hr_list_node_t *hr_list_pop_back(hr_list_t *list);
bool hr_list_remove(hr_list_t *list, hr_list_node_t *node);
bool hr_list_validate(const hr_list_t *list);

#endif
