#ifndef AK_FSM_H
#define AK_FSM_H

#include <stdbool.h>
#include <stdint.h>

#include "message.h"

typedef struct ak_fsm ak_fsm_t;

typedef bool (*ak_fsm_dispatch_fn_t)(ak_fsm_t *fsm, const ak_message_t *message);

struct ak_fsm
{
    uint8_t state;
    void *context;
    ak_fsm_dispatch_fn_t dispatch;
};

void ak_fsm_init(ak_fsm_t *fsm, uint8_t initial_state, void *context, ak_fsm_dispatch_fn_t dispatch);

bool ak_fsm_handle(ak_fsm_t *fsm, const ak_message_t *message);

void ak_fsm_transition(ak_fsm_t *fsm, uint8_t next_state);

#endif
