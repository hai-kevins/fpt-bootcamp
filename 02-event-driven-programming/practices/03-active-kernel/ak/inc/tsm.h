#ifndef AK_TSM_H
#define AK_TSM_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "message.h"

typedef void (*ak_tsm_action_t)(void *context, const ak_message_t *message);

typedef struct
{
    uint8_t current_state;
    uint16_t signal;
    uint8_t next_state;
    ak_tsm_action_t action;
} ak_tsm_transition_t;

typedef struct
{
    uint8_t state;
    void *context;
    const ak_tsm_transition_t *transitions;
    size_t transition_count;
} ak_tsm_t;

void ak_tsm_init(ak_tsm_t *machine, uint8_t initial_state, void *context, const ak_tsm_transition_t *transitions,
    size_t transition_count);

bool ak_tsm_handle(ak_tsm_t *machine, const ak_message_t *message);

#endif
