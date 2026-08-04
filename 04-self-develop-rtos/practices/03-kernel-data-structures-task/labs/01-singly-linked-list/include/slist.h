#ifndef SLIST_H
#define SLIST_H

#include <stdbool.h>
#include <stddef.h>

typedef struct slist_node
{
    struct slist_node * next;
    int value;
    bool linked;
} slist_node_t;

typedef struct
{
    slist_node_t *head;
    slist_node_t *tail;
    size_t count;
} slist_t;

void slist_init(slist_t *list);
void slist_node_init(slist_node_t *node, int value);
bool slist_push_front(slist_t *list, slist_node_t *node);
bool slist_push_back(slist_t *list, slist_node_t *node);
slist_node_t *slist_pop_front(slist_t *list);
slist_node_t *slist_find(const slist_t *list, int value);
bool slist_validate(const slist_t *list);

#endif
