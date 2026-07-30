#ifndef ED_ACTIVE_OBJECT_H
#define ED_ACTIVE_OBJECT_H

#include "event.h"
#include "mailbox.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct ed_active_object ed_active_object_t;

typedef void (*ed_event_handler_t)(ed_active_object_t *object,
                                   const ed_event_t *event);

struct ed_active_object
{
    uint8_t id;
    uint8_t priority;
    const char *name;
    ed_mailbox_t mailbox;
    ed_event_handler_t handler;
    void *context;
    uint32_t dispatch_count;
};

bool ed_active_object_init(ed_active_object_t *object,
                           uint8_t id,
                           uint8_t priority,
                           const char *name,
                           size_t mailbox_capacity,
                           ed_event_handler_t handler,
                           void *context);
bool ed_active_object_post(ed_active_object_t *object,
                           ed_event_t *event);
bool ed_active_object_ready(const ed_active_object_t *object);

#endif
