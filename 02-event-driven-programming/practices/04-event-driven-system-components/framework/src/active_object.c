#include "active_object.h"

bool ed_active_object_init(ed_active_object_t *object, uint8_t id, uint8_t priority, const char *name, size_t mailbox_capacity,
    ed_event_handler_t handler, void *context)
{
    if ((object == NULL) || (handler == NULL) || (name == NULL))
    {
        return false;
    }

    object->id = id;
    object->priority = priority;
    object->name = name;
    object->handler = handler;
    object->context = context;
    object->dispatch_count = 0U;

    return ed_mailbox_init(&object->mailbox, mailbox_capacity, ED_MAILBOX_DROP_NEWEST);
}

bool ed_active_object_post(ed_active_object_t *object, ed_event_t *event)
{
    if ((object == NULL) || (event == NULL))
    {
        return false;
    }

    event->destination = object->id;
    return ed_mailbox_post(&object->mailbox, event);
}

bool ed_active_object_ready(const ed_active_object_t *object)
{
    return (object != NULL) && !ed_mailbox_is_empty(&object->mailbox);
}
