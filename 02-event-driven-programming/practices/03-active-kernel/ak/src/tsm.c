#include "tsm.h"

#include "ak_port.h"
#include "event_record.h"

void ak_tsm_init(ak_tsm_t *machine, uint8_t initial_state, void *context, const ak_tsm_transition_t *transitions,
    size_t transition_count)
{
    if (machine != 0)
    {
        machine->state = initial_state;
        machine->context = context;
        machine->transitions = transitions;
        machine->transition_count = transition_count;
    }
}

bool ak_tsm_handle(ak_tsm_t *machine, const ak_message_t *message)
{
    if ((machine == 0) || (message == 0) || (machine->transitions == 0))
    {
        return false;
    }

    for (size_t i = 0U; i < machine->transition_count; i++)
    {
        const ak_tsm_transition_t *transition = &machine->transitions[i];
        if ((transition->current_state == machine->state) && (transition->signal == message->signal))
        {
            const uint8_t previous = machine->state;
            if (transition->action != 0)
            {
                transition->action(machine->context, message);
            }
            machine->state = transition->next_state;

            ak_event_record_t record =
            {
                .timestamp = ak_port_time_now_ms(),
                .signal = message->signal,
                .value = transition->next_state,
                .type = AK_RECORD_STATE_TRANSITION,
                .source = previous,
                .destination = transition->next_state,
                .state = transition->next_state
            };
            ak_event_record_write(&record);
            return true;
        }
    }

    return false;
}
