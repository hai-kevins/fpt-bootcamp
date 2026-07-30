#include "hsm.h"

bool ed_hsm_init(ed_hsm_t *hsm,
                 uint8_t initial_state,
                 const uint8_t *parents,
                 size_t state_count,
                 ed_hsm_state_handler_t handler,
                 void *context)
{
    if ((hsm == NULL) || (parents == NULL) ||
        (handler == NULL) || (state_count == 0U) ||
        (initial_state >= state_count))
    {
        return false;
    }

    hsm->current_state = initial_state;
    hsm->parents = parents;
    hsm->state_count = state_count;
    hsm->handler = handler;
    hsm->context = context;
    hsm->handled_count = 0U;
    hsm->bubbled_count = 0U;
    hsm->unhandled_count = 0U;
    return true;
}

bool ed_hsm_dispatch(ed_hsm_t *hsm,
                     const ed_event_t *event)
{
    uint8_t state;
    size_t depth = 0U;

    if ((hsm == NULL) || (event == NULL))
    {
        return false;
    }

    state = hsm->current_state;

    while ((state != ED_HSM_NO_STATE) &&
           (state < hsm->state_count) &&
           (depth < hsm->state_count))
    {
        uint8_t next_state = hsm->current_state;

        if (hsm->handler(hsm->context, state, event, &next_state))
        {
            if (next_state < hsm->state_count)
            {
                hsm->current_state = next_state;
            }
            hsm->handled_count++;
            return true;
        }

        state = hsm->parents[state];
        depth++;
        if (state != ED_HSM_NO_STATE)
        {
            hsm->bubbled_count++;
        }
    }

    hsm->unhandled_count++;
    return false;
}
