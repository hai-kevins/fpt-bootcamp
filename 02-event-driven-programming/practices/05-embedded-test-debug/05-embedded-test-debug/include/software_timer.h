#ifndef SOFTWARE_TIMER_H
#define SOFTWARE_TIMER_H

#include "event.h"
#include "event_queue.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define SOFTWARE_TIMER_MAX_COUNT (8U)

typedef struct
{
    bool active;
    bool periodic;
    uint32_t deadline_ms;
    uint32_t period_ms;
    event_t event;
} software_timer_entry_t;

typedef struct
{
    software_timer_entry_t entries[SOFTWARE_TIMER_MAX_COUNT];
    uint32_t arm_count;
    uint32_t cancel_count;
    uint32_t expiry_count;
    uint32_t post_failures;
} software_timer_service_t;

void software_timer_init(
    software_timer_service_t *service
);

bool software_timer_start(
    software_timer_service_t *service,
    size_t timer_id,
    uint32_t now_ms,
    uint32_t delay_ms,
    bool periodic,
    const event_t *event
);

bool software_timer_cancel(
    software_timer_service_t *service,
    size_t timer_id
);

void software_timer_process(
    software_timer_service_t *service,
    uint32_t now_ms,
    event_queue_t *queue
);

#endif
