#include "app_sm.h"
#include "test_assert.h"
#include "test_cases.h"

bool test_app_transitions(void)
{
    app_sm_t app;
    event_t start =
    {
        .signal = EVENT_SIGNAL_APP_START
    };
    event_t stop =
    {
        .signal = EVENT_SIGNAL_APP_STOP
    };

    app_sm_init(&app);

    TEST_ASSERT_TRUE(app_sm_dispatch(&app, &start));
    TEST_ASSERT_EQ(APP_STATE_RUNNING, app.state);

    TEST_ASSERT_TRUE(app_sm_dispatch(&app, &stop));
    TEST_ASSERT_EQ(APP_STATE_IDLE, app.state);
    TEST_ASSERT_EQ(2U, app.transition_count);
    return true;
}

bool test_app_error_recovery(void)
{
    app_sm_t app;
    event_t start =
    {
        .signal = EVENT_SIGNAL_APP_START
    };
    event_t fail =
    {
        .signal = EVENT_SIGNAL_APP_FAIL
    };
    event_t reset =
    {
        .signal = EVENT_SIGNAL_APP_RESET
    };

    app_sm_init(&app);

    TEST_ASSERT_TRUE(app_sm_dispatch(&app, &start));
    TEST_ASSERT_TRUE(app_sm_dispatch(&app, &fail));
    TEST_ASSERT_EQ(APP_STATE_ERROR, app.state);
    TEST_ASSERT_TRUE(app_sm_dispatch(&app, &reset));
    TEST_ASSERT_EQ(APP_STATE_IDLE, app.state);
    return true;
}
