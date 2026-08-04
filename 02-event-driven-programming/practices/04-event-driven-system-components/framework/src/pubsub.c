#include "pubsub.h"

#include <string.h>

void ed_pubsub_init(ed_pubsub_t *pubsub, ed_event_pool_t *pool)
{
    if (pubsub != NULL)
    {
        (void) memset(pubsub, 0, sizeof (*pubsub));
        pubsub->pool = pool;
    }
}

bool ed_pubsub_subscribe(ed_pubsub_t *pubsub, ed_signal_t signal, ed_active_object_t *object)
{
    if ((pubsub == NULL) || (object == NULL) || (signal >= ED_PUBSUB_MAX_SIGNALS))
    {
        return false;
    }

    size_t *count = &pubsub->subscriber_count[signal];
    if (*count >= ED_PUBSUB_MAX_SUBSCRIBERS)
    {
        return false;
    }

    for (size_t i = 0U; i < *count; i++)
    {
        if (pubsub->subscribers[signal][i] == object)
        {
            return false;
        }
    }

    pubsub->subscribers[signal][*count] = object;
    (*count)++;
    return true;
}

size_t ed_pubsub_publish(ed_pubsub_t *pubsub, const ed_event_t *prototype)
{
    size_t delivered = 0U;

    if ((pubsub == NULL) || (prototype == NULL) || (pubsub->pool == NULL) || (prototype->signal >= ED_PUBSUB_MAX_SIGNALS))
    {
        return 0U;
    }

    pubsub->publish_count++;

    const size_t count = pubsub->subscriber_count[prototype->signal];
    for (size_t i = 0U; i < count; i++)
    {
        ed_active_object_t *object = pubsub->subscribers[prototype->signal][i];
        ed_event_t *copy = ed_event_pool_allocate(pubsub->pool, prototype->signal, prototype->source, object->id);
        if (copy == NULL)
        {
            pubsub->delivery_failure_count++;
            continue;
        }

        copy->sequence = prototype->sequence;
        if (!ed_event_set_payload(copy, prototype->payload, prototype->payload_length) || !ed_active_object_post(object,
        copy))
        {
            (void) ed_event_pool_release(pubsub->pool, copy);
            pubsub->delivery_failure_count++;
            continue;
        }

        delivered++;
        pubsub->delivery_count++;
    }

    return delivered;
}
