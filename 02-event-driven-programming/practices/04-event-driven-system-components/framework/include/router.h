#ifndef ED_ROUTER_H
#define ED_ROUTER_H

#include "active_object.h"
#include "event_pool.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define ED_ROUTER_MAX_OBJECTS (16U)

typedef bool (*ed_remote_send_t)(void *context, const ed_event_t *event);

typedef struct
{
    uint8_t local_node;
    ed_active_object_t *objects[ED_ROUTER_MAX_OBJECTS];
    size_t object_count;
    ed_remote_send_t remote_send;
    void *remote_context;
    ed_event_pool_t *pool;
    uint32_t local_post_count;
    uint32_t remote_post_count;
    uint32_t route_failure_count;
} ed_router_t;

void ed_router_init(ed_router_t *router, uint8_t local_node, ed_event_pool_t *pool, ed_remote_send_t remote_send,
    void *remote_context);
bool ed_router_register(ed_router_t *router, ed_active_object_t *object);
bool ed_router_post(ed_router_t *router, uint8_t destination_node, ed_event_t *event);

#endif
