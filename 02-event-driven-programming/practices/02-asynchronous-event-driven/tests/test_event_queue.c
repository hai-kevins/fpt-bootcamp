#include "event_queue.h"
#include "test.h"

bool test_event_queue_fifo(void)
{
    event_queue_t queue;
    event_t output;

    event_queue_init(&queue);

    for (uint32_t i = 0UL; i < 4UL; i++)
    {
        const event_t event =
        {
            .timestamp_ms = i,
            .parameter = i + 10UL,
            .signal = (uint16_t)SIGNAL_SENSOR_READY,
            .source = (uint8_t)EVENT_SOURCE_TEST,
            .destination = (uint8_t)EVENT_DESTINATION_APP
        };

        TEST_ASSERT_TRUE(event_queue_post(&queue, &event));
    }

    for (uint32_t i = 0UL; i < 4UL; i++)
    {
        TEST_ASSERT_TRUE(event_queue_get(&queue, &output));
        TEST_ASSERT_EQ_U32(i + 10UL, output.parameter);
    }

    TEST_ASSERT_TRUE(event_queue_is_empty(&queue));
    return true;
}

bool test_event_queue_wrap_and_overflow(void)
{
    event_queue_t queue;
    event_t event = {0};
    event_t output;

    event_queue_init(&queue);

    for (uint32_t i = 0UL;
         i < (uint32_t)EVENT_QUEUE_CAPACITY;
         i++)
    {
        event.parameter = i;
        TEST_ASSERT_TRUE(event_queue_post(&queue, &event));
    }

    TEST_ASSERT_TRUE(event_queue_is_full(&queue));
    TEST_ASSERT_TRUE(!event_queue_post(&queue, &event));
    TEST_ASSERT_EQ_U32(1UL, queue.overflow_count);

    for (uint32_t i = 0UL; i < 8UL; i++)
    {
        TEST_ASSERT_TRUE(event_queue_get(&queue, &output));
    }

    for (uint32_t i = 0UL; i < 8UL; i++)
    {
        event.parameter = 100UL + i;
        TEST_ASSERT_TRUE(event_queue_post(&queue, &event));
    }

    TEST_ASSERT_EQ_U32(
        EVENT_QUEUE_CAPACITY,
        queue.high_water_mark
    );

    return true;
}
