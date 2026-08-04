#ifndef ED_HSM_H
#define ED_HSM_H

#include "event.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define ED_HSM_NO_STATE (UINT8_MAX)

typedef bool (*ed_hsm_state_handler_t)(void *context, uint8_t state, const ed_event_t *event, uint8_t *next_state);

typedef struct
{
    uint8_t current_state;
    const uint8_t *parents;
    size_t state_count;
    ed_hsm_state_handler_t handler;
    void *context;
    uint32_t handled_count;
    uint32_t bubbled_count;
    uint32_t unhandled_count;
} ed_hsm_t;

bool ed_hsm_init(ed_hsm_t *hsm, uint8_t initial_state, const uint8_t *parents, size_t state_count, ed_hsm_state_handler_t handler,
    void *context);
bool ed_hsm_dispatch(ed_hsm_t *hsm, const ed_event_t *event);

#endif
