#ifndef RTOS_LIST_H
#define RTOS_LIST_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct rtos_list_node
{
    struct rtos_list_node *previous;
    struct rtos_list_node *next;
    const void *owner;
} rtos_list_node_t;

typedef struct
{
    rtos_list_node_t *head;
    rtos_list_node_t *tail;
    size_t count;
} rtos_list_t;

#define RTOS_CONTAINER_OF(pointer, type, member) \
    ((type *)((uint8_t *)(pointer) - offsetof(type, member)))

void rtos_list_init(rtos_list_t *list);
void rtos_list_node_init(rtos_list_node_t *node);
bool rtos_list_is_empty(const rtos_list_t *list);
size_t rtos_list_count(const rtos_list_t *list);
rtos_list_node_t *rtos_list_front(const rtos_list_t *list);
rtos_list_node_t *rtos_list_back(const rtos_list_t *list);
bool rtos_list_push_front(rtos_list_t *list, rtos_list_node_t *node);
bool rtos_list_push_back(rtos_list_t *list, rtos_list_node_t *node);
bool rtos_list_insert_before(rtos_list_t *list, rtos_list_node_t *position, rtos_list_node_t *node);
bool rtos_list_insert_after(rtos_list_t *list, rtos_list_node_t *position, rtos_list_node_t *node);
rtos_list_node_t *rtos_list_pop_front(rtos_list_t *list);
rtos_list_node_t *rtos_list_pop_back(rtos_list_t *list);
bool rtos_list_remove(rtos_list_t *list, rtos_list_node_t *node);
bool rtos_list_validate(const rtos_list_t *list);

#endif
