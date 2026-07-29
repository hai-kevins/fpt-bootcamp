#include "app.h"
#include "test.h"

bool test_app_state_machine(void)
{
    const event_t start =
    {
        .timestamp_ms = 0UL,
        .signal = (uint16_t)SIGNAL_SYSTEM_START
    };

    const event_t press =
    {
        .timestamp_ms = 10UL,
        .signal = (uint16_t)SIGNAL_BUTTON_PRESSED
    };

    const event_t error =
    {
        .timestamp_ms = 20UL,
        .signal = (uint16_t)SIGNAL_ERROR_DETECTED
    };

    const event_t reset =
    {
        .timestamp_ms = 30UL,
        .signal = (uint16_t)SIGNAL_RESET_REQUEST
    };

    test_reset_framework();

    app_event_handler(&start);
    TEST_ASSERT_EQ_INT(APP_STATE_IDLE, app_get_state());

    app_event_handler(&press);
    TEST_ASSERT_EQ_INT(APP_STATE_ACTIVE, app_get_state());

    app_event_handler(&error);
    TEST_ASSERT_EQ_INT(APP_STATE_ERROR, app_get_state());

    app_event_handler(&reset);
    TEST_ASSERT_EQ_INT(APP_STATE_IDLE, app_get_state());

    return true;
}
