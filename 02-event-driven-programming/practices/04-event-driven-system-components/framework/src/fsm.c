#include "fsm.h"

void ed_fsm_init(ed_fsm_t *fsm,
                 uint8_t initial_state,
                 const ed_fsm_transition_t *transitions,
                 size_t transition_count,
                 void *context)
{
    if (fsm == NULL)
    {
        return;
    }

    fsm->state = initial_state;
    fsm->transitions = transitions;
    fsm->transition_count = transition_count;
    fsm->context = context;
    fsm->handled_count = 0U;
    fsm->unhandled_count = 0U;
}

bool ed_fsm_dispatch(ed_fsm_t *fsm,
                     const ed_event_t *event)
{
    if ((fsm == NULL) || (event == NULL) ||
        (fsm->transitions == NULL))
    {
        return false;
    }

    for (size_t i = 0U; i < fsm->transition_count; i++)
    {
        const ed_fsm_transition_t *transition = &fsm->transitions[i];

        if ((transition->state == fsm->state) &&
            (transition->signal == event->signal))
        {
            if (transition->action != NULL)
            {
                transition->action(fsm->context, event);
            }
            fsm->state = transition->next_state;
            fsm->handled_count++;
            return true;
        }
    }

    fsm->unhandled_count++;
    return false;
}
