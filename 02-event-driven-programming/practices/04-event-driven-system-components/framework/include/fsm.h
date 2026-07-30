#ifndef ED_FSM_H
#define ED_FSM_H

#include "event.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef void (*ed_fsm_action_t)(void *context,
                                const ed_event_t *event);

typedef struct
{
    uint8_t state;
    ed_signal_t signal;
    uint8_t next_state;
    ed_fsm_action_t action;
} ed_fsm_transition_t;

typedef struct
{
    uint8_t state;
    const ed_fsm_transition_t *transitions;
    size_t transition_count;
    void *context;
    uint32_t handled_count;
    uint32_t unhandled_count;
} ed_fsm_t;

void ed_fsm_init(ed_fsm_t *fsm,
                 uint8_t initial_state,
                 const ed_fsm_transition_t *transitions,
                 size_t transition_count,
                 void *context);
bool ed_fsm_dispatch(ed_fsm_t *fsm,
                     const ed_event_t *event);

#endif
