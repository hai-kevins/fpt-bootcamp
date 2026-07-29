#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "event.h"

#include <stdbool.h>
#include <stdint.h>

typedef void (*event_handler_t)(const event_t *event);

typedef struct
{
    uint32_t posted;
    uint32_t dispatched;
    uint32_t dropped;
    uint32_t invalid_destination;
    uint16_t pending;
    uint16_t high_water_mark;
} dispatcher_stats_t;

void dispatcher_init(void);
bool dispatcher_register(
    event_destination_t destination,
    event_handler_t handler
);
bool dispatcher_post_event(const event_t *event);
bool dispatcher_post(
    event_source_t source,
    event_destination_t destination,
    signal_t signal,
    uint32_t parameter,
    uint32_t timestamp_ms
);
bool dispatcher_try_get(event_t *event);
bool dispatcher_dispatch_once(void);
void dispatcher_run_until_empty(void);
uint16_t dispatcher_pending(void);
dispatcher_stats_t dispatcher_get_stats(void);

#endif
