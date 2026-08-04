#include "test.h"
#include "event_record.h"
#include "fsm.h"
#include "tsm.h"

static bool fsm_dispatch(ak_fsm_t *fsm, const ak_message_t *message)
{
    if ((fsm->state == 0U) && (message->signal == 1U))
    {
        ak_fsm_transition(fsm, 1U);
        return true;
    }
    return false;
}

bool test_fsm_and_tsm(void)
{
    ak_event_record_init();
    ak_message_t message =
    {
        .signal = 1U
    };
    ak_fsm_t fsm;
    ak_fsm_init(&fsm, 0U, 0, fsm_dispatch);
    TEST_ASSERT_TRUE(ak_fsm_handle(&fsm, &message));
    TEST_ASSERT_EQ(1U, fsm.state);

    const ak_tsm_transition_t transitions[] =
    {
        {
            0U, 1U, 2U, 0
        }
    };
    ak_tsm_t tsm;
    ak_tsm_init(&tsm, 0U, 0, transitions, 1U);
    TEST_ASSERT_TRUE(ak_tsm_handle(&tsm, &message));
    TEST_ASSERT_EQ(2U, tsm.state);
    return true;
}
