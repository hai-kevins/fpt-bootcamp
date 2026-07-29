#include "led.h"
#include "software_timer.h"
#include "test.h"

bool test_led_state_machine(void)
{
    const event_t start =
    {
        .timestamp_ms = 0UL,
        .parameter = 100UL,
        .signal = (uint16_t)SIGNAL_LED_BLINK_START,
        .source = (uint8_t)EVENT_SOURCE_TEST,
        .destination = (uint8_t)EVENT_DESTINATION_LED
    };

    const event_t stop =
    {
        .timestamp_ms = 200UL,
        .parameter = 0UL,
        .signal = (uint16_t)SIGNAL_LED_BLINK_STOP,
        .source = (uint8_t)EVENT_SOURCE_TEST,
        .destination = (uint8_t)EVENT_DESTINATION_LED
    };

    test_reset_framework();

    led_event_handler(&start);

    TEST_ASSERT_EQ_INT(LED_STATE_BLINKING, led_get_state());
    TEST_ASSERT_EQ_U32(100UL, led_get_blink_period_ms());
    TEST_ASSERT_TRUE(fake_get_led());
    TEST_ASSERT_TRUE(
        software_timer_is_active(
            SOFTWARE_TIMER_LED_BLINK
        )
    );

    software_timer_process(100UL);
    test_dispatch_all();
    TEST_ASSERT_TRUE(!fake_get_led());

    led_event_handler(&stop);
    TEST_ASSERT_EQ_INT(LED_STATE_OFF, led_get_state());
    TEST_ASSERT_TRUE(!fake_get_led());

    return true;
}
