#include "router.h"

#include <string.h>

void ed_router_init(ed_router_t *router,
                    uint8_t local_node,
                    ed_event_pool_t *pool,
                    ed_remote_send_t remote_send,
                    void *remote_context)
{
    if (router != NULL)
    {
        (void)memset(router, 0, sizeof(*router));
        router->local_node = local_node;
        router->pool = pool;
        router->remote_send = remote_send;
        router->remote_context = remote_context;
    }
}

bool ed_router_register(ed_router_t *router,
                        ed_active_object_t *object)
{
    if ((router == NULL) || (object == NULL) ||
        (router->object_count >= ED_ROUTER_MAX_OBJECTS))
    {
        return false;
    }

    router->objects[router->object_count] = object;
    router->object_count++;
    return true;
}

bool ed_router_post(ed_router_t *router,
                    uint8_t destination_node,
                    ed_event_t *event)
{
    if ((router == NULL) || (event == NULL))
    {
        return false;
    }

    if (destination_node == router->local_node)
    {
        for (size_t i = 0U; i < router->object_count; i++)
        {
            if (router->objects[i]->id == event->destination)
            {
                if (ed_active_object_post(router->objects[i], event))
                {
                    router->local_post_count++;
                    return true;
                }
                break;
            }
        }
    }
    else if ((router->remote_send != NULL) &&
             router->remote_send(router->remote_context, event))
    {
        router->remote_post_count++;
        if (router->pool != NULL)
        {
            (void)ed_event_pool_release(router->pool, event);
        }
        return true;
    }

    router->route_failure_count++;
    return false;
}
