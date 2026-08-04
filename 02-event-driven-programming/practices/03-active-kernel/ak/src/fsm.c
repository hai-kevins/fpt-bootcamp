#include "fsm.h"

#include "ak_port.h"
#include "event_record.h"

void ak_fsm_init(ak_fsm_t *fsm, uint8_t initial_state, void *context, ak_fsm_dispatch_fn_t dispatch)
{
    if (fsm != 0)
    {
        fsm->state = initial_state;
        fsm->context = context;
        fsm->dispatch = dispatch;
    }
}

bool ak_fsm_handle(ak_fsm_t *fsm, const ak_message_t *message)
{
    if ((fsm == 0) || (message == 0) || (fsm->dispatch == 0))
    {
        return false;
    }
    return fsm->dispatch(fsm, message);
}

void ak_fsm_transition(ak_fsm_t *fsm, uint8_t next_state)
{
    if (fsm != 0)
    {
        ak_event_record_t record =
        {
            .timestamp = ak_port_time_now_ms(),
            .signal = 0U,
            .value = next_state,
            .type = AK_RECORD_STATE_TRANSITION,
            .source = fsm->state,
            .destination = next_state,
            .state = next_state
        };
        ak_event_record_write(&record);
        fsm->state = next_state;
    }
}
