#ifndef SOFTWARE_TIMER_H
#define SOFTWARE_TIMER_H

#include "event.h"

#include <stdbool.h>
#include <stdint.h>

typedef enum
{
    SOFTWARE_TIMER_BUTTON_DEBOUNCE = 0,
    SOFTWARE_TIMER_LED_BLINK,
    SOFTWARE_TIMER_APP_TIMEOUT,
    SOFTWARE_TIMER_COUNT
} software_timer_id_t;

void software_timer_init(void);

bool software_timer_start(
    software_timer_id_t id,
    event_destination_t destination,
    signal_t signal,
    uint32_t parameter,
    uint32_t delay_ms,
    bool periodic,
    uint32_t now_ms
);

bool software_timer_cancel(software_timer_id_t id);
bool software_timer_is_active(software_timer_id_t id);
void software_timer_process(uint32_t now_ms);

#endif
