#ifndef DEMO_AO_H
#define DEMO_AO_H

#include "active_object.h"

typedef struct
{
    ed_active_object_t super;
    uint32_t handled;
    uint32_t last_value;
} demo_ao_t;

bool demo_ao_init(demo_ao_t *object, uint8_t id, uint8_t priority, const char *name);

#endif
