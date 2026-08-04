#include "event.h"

#include <string.h>

void ed_event_init_static(ed_event_t *event, ed_signal_t signal, uint8_t source, uint8_t destination)
{
    if (event == NULL)
    {
        return;
    }

    (void) memset(event, 0, sizeof (*event));
    event->signal = signal;
    event->source = source;
    event->destination = destination;
    event->pool_index = UINT8_MAX;
}

bool ed_event_set_payload(ed_event_t *event, const void *payload, size_t length)
{
    if ((event == NULL) || ((payload == NULL) && (length > 0U)) || (length > ED_EVENT_PAYLOAD_MAX))
    {
        return false;
    }

    if (length > 0U)
    {
        (void) memcpy(event->payload, payload, length);
    }

    event->payload_length = (uint8_t) length;
    return true;
}
