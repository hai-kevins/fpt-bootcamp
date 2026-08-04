#include "event_queue.h"
#include "fake_time.h"
#include "software_timer.h"
#include "test_assert.h"
#include "test_cases.h"

bool test_timer_one_shot(void)
{
    fake_time_t time_source;
    event_queue_t queue;
    software_timer_service_t timers;
    event_t timeout =
    {
        .signal = EVENT_SIGNAL_APP_TIMEOUT
    };
    event_t output =
    {
        0
    };

    fake_time_reset(&time_source);
    TEST_ASSERT_TRUE(event_queue_init(&queue, 4U));
    software_timer_init(&timers);

    TEST_ASSERT_TRUE(software_timer_start(&timers, 0U, fake_time_now(&time_source), 100U, false, &timeout));

    fake_time_advance(&time_source, 99U);
    software_timer_process(&timers, fake_time_now(&time_source), &queue);
    TEST_ASSERT_TRUE(event_queue_is_empty(&queue));

    fake_time_advance(&time_source, 1U);
    software_timer_process(&timers, fake_time_now(&time_source), &queue);

    TEST_ASSERT_TRUE(event_queue_get(&queue, &output));
    TEST_ASSERT_EQ(EVENT_SIGNAL_APP_TIMEOUT, output.signal);
    TEST_ASSERT_FALSE(timers.entries[0].active);
    return true;
}

bool test_timer_periodic(void)
{
    fake_time_t time_source;
    event_queue_t queue;
    software_timer_service_t timers;
    event_t event =
    {
        .signal = EVENT_SIGNAL_DIAGNOSTIC
    };

    fake_time_reset(&time_source);
    TEST_ASSERT_TRUE(event_queue_init(&queue, 8U));
    software_timer_init(&timers);

    TEST_ASSERT_TRUE(software_timer_start(&timers, 1U, 0U, 10U, true, &event));

    fake_time_advance(&time_source, 35U);
    software_timer_process(&timers, fake_time_now(&time_source), &queue);

    TEST_ASSERT_EQ(1U, queue.count);
    TEST_ASSERT_EQ(40U, timers.entries[1].deadline_ms);
    TEST_ASSERT_TRUE(timers.entries[1].active);
    return true;
}
