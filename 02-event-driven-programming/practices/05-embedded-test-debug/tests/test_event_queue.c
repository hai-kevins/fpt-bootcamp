#include "event_queue.h"
#include "test_assert.h"
#include "test_cases.h"

bool test_queue_fifo(void)
{
    event_queue_t queue;
    event_t first =
    {
        .signal = 1U
    };
    event_t second =
    {
        .signal = 2U
    };
    event_t output =
    {
        0
    };

    TEST_ASSERT_TRUE(event_queue_init(&queue, 2U));
    TEST_ASSERT_TRUE(event_queue_post(&queue, &first));
    TEST_ASSERT_TRUE(event_queue_post(&queue, &second));

    TEST_ASSERT_TRUE(event_queue_get(&queue, &output));
    TEST_ASSERT_EQ(1U, output.signal);

    TEST_ASSERT_TRUE(event_queue_get(&queue, &output));
    TEST_ASSERT_EQ(2U, output.signal);

    TEST_ASSERT_TRUE(event_queue_is_empty(&queue));
    TEST_ASSERT_EQ(2U, queue.high_water_mark);
    return true;
}

bool test_queue_overflow(void)
{
    event_queue_t queue;
    event_t event =
    {
        .signal = 1U
    };

    TEST_ASSERT_TRUE(event_queue_init(&queue, 1U));
    TEST_ASSERT_TRUE(event_queue_post(&queue, &event));
    TEST_ASSERT_FALSE(event_queue_post(&queue, &event));
    TEST_ASSERT_EQ(1U, queue.overflow_count);
    return true;
}
