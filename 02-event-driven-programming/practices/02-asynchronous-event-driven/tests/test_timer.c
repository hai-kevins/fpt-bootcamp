#include "dispatcher.h"
#include "software_timer.h"
#include "test.h"

bool test_timer_one_shot(void)
{
    event_t event;

    test_reset_framework();

    TEST_ASSERT_TRUE(software_timer_start(SOFTWARE_TIMER_APP_TIMEOUT, EVENT_DESTINATION_APP, SIGNAL_SENSOR_READY,
        77UL, 10UL, false, 100UL));

    software_timer_process(109UL);
    TEST_ASSERT_EQ_U32(0UL, dispatcher_pending());

    software_timer_process(110UL);
    TEST_ASSERT_TRUE(dispatcher_try_get(&event));
    TEST_ASSERT_EQ_U32(SIGNAL_SENSOR_READY, event.signal);
    TEST_ASSERT_EQ_U32(77UL, event.parameter);
    TEST_ASSERT_TRUE(!software_timer_is_active(SOFTWARE_TIMER_APP_TIMEOUT));

    return true;
}

bool test_timer_periodic(void)
{
    event_t event;

    test_reset_framework();

    TEST_ASSERT_TRUE(software_timer_start(SOFTWARE_TIMER_LED_BLINK, EVENT_DESTINATION_LED, SIGNAL_LED_TIMEOUT,
        0UL, 5UL, true, 0UL));

    software_timer_process(5UL);
    TEST_ASSERT_TRUE(dispatcher_try_get(&event));

    software_timer_process(10UL);
    TEST_ASSERT_TRUE(dispatcher_try_get(&event));

    TEST_ASSERT_TRUE(software_timer_is_active(SOFTWARE_TIMER_LED_BLINK));

    return true;
}
