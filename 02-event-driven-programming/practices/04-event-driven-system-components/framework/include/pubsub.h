#ifndef ED_PUBSUB_H
#define ED_PUBSUB_H

#include "active_object.h"
#include "event_pool.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define ED_PUBSUB_MAX_SIGNALS     (64U)
#define ED_PUBSUB_MAX_SUBSCRIBERS (8U)

typedef struct
{
    ed_active_object_t *subscribers[ED_PUBSUB_MAX_SIGNALS]
                                      [ED_PUBSUB_MAX_SUBSCRIBERS];
    size_t subscriber_count[ED_PUBSUB_MAX_SIGNALS];
    ed_event_pool_t *pool;
    uint32_t publish_count;
    uint32_t delivery_count;
    uint32_t delivery_failure_count;
} ed_pubsub_t;

void ed_pubsub_init(ed_pubsub_t *pubsub,
                    ed_event_pool_t *pool);
bool ed_pubsub_subscribe(ed_pubsub_t *pubsub,
                         ed_signal_t signal,
                         ed_active_object_t *object);
size_t ed_pubsub_publish(ed_pubsub_t *pubsub,
                         const ed_event_t *prototype);

#endif
