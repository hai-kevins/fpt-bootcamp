#include "demo_ao.h"

#include <string.h>

static void demo_handler(ed_active_object_t *base,
                         const ed_event_t *event)
{
    demo_ao_t *object = (demo_ao_t *)base->context;
    object->handled++;
    if (event->payload_length >= sizeof(uint32_t))
    {
        uint32_t value = 0U;
        (void)memcpy(&value, event->payload, sizeof(value));
        object->last_value = value;
    }
}

bool demo_ao_init(demo_ao_t *object,
                  uint8_t id,
                  uint8_t priority,
                  const char *name)
{
    if (object == NULL)
    {
        return false;
    }

    object->handled = 0U;
    object->last_value = 0U;
    return ed_active_object_init(&object->super,
                                 id,
                                 priority,
                                 name,
                                 8U,
                                 demo_handler,
                                 object);
}
