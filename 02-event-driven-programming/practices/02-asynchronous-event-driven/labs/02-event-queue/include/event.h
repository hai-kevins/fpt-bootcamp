#ifndef LAB02_EVENT_H
#define LAB02_EVENT_H

#include <stdint.h>

typedef struct
{
    uint32_t sequence;
    uint16_t signal;
} event_t;

#endif
