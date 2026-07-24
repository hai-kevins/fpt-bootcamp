#ifndef DLIST_H
#define DLIST_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct dlist_node
{
    struct dlist_node *previous;
    struct dlist_node *next;
    const void *owner;
} dlist_node_t;

typedef struct
{
    dlist_node_t *head;
    dlist_node_t *tail;
    size_t count;
} dlist_t;

#define DLIST_CONTAINER_OF(pointer, type, member) \
    ((type *)((uint8_t *)(pointer) - offsetof(type, member)))

void dlist_init(dlist_t *list);
void dlist_node_init(dlist_node_t *node);
bool dlist_is_empty(const dlist_t *list);
size_t dlist_count(const dlist_t *list);
dlist_node_t *dlist_front(const dlist_t *list);
dlist_node_t *dlist_back(const dlist_t *list);
bool dlist_push_front(dlist_t *list, dlist_node_t *node);
bool dlist_push_back(dlist_t *list, dlist_node_t *node);
bool dlist_insert_before(dlist_t *list, dlist_node_t *position, dlist_node_t *node);
bool dlist_insert_after(dlist_t *list, dlist_node_t *position, dlist_node_t *node);
dlist_node_t *dlist_pop_front(dlist_t *list);
dlist_node_t *dlist_pop_back(dlist_t *list);
bool dlist_remove(dlist_t *list, dlist_node_t *node);
bool dlist_validate(const dlist_t *list);

#endif
