#include "button.h"
#include "dispatcher.h"
#include "software_timer.h"
#include "test.h"

bool test_button_debounce_press(void)
{
    event_t edge =
    {
        .timestamp_ms = 10UL,
        .parameter = 0UL,
        .signal = (uint16_t) SIGNAL_BUTTON_EDGE,
        .source = (uint8_t) EVENT_SOURCE_ISR,
        .destination = (uint8_t) EVENT_DESTINATION_BUTTON
    };

    test_reset_framework();
    fake_set_button(false);
    button_init();

    button_event_handler(&edge);
    fake_set_button(true);

    software_timer_process(29UL);
    test_dispatch_all();
    TEST_ASSERT_TRUE(!button_is_stably_pressed());

    software_timer_process(30UL);
    test_dispatch_all();

    TEST_ASSERT_TRUE(button_is_stably_pressed());
    return true;
}
