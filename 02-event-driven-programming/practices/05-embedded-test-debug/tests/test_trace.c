#include "event_trace.h"
#include "test_assert.h"
#include "test_cases.h"

bool test_trace_wraparound(void)
{
    event_trace_t trace;
    event_trace_record_t record =
    {
        0
    };
    event_trace_record_t oldest =
    {
        0
    };

    event_trace_init(&trace);

    for (uint32_t i = 0U; i < EVENT_TRACE_CAPACITY + 3U; i++)
    {
        record.timestamp_ms = i;
        record.signal = (uint16_t) i;
        event_trace_write(&trace, &record);
    }

    TEST_ASSERT_EQ(EVENT_TRACE_CAPACITY, trace.count);
    TEST_ASSERT_EQ(3U, trace.overwritten_count);
    TEST_ASSERT_TRUE(event_trace_get_oldest(&trace, 0U, &oldest));
    TEST_ASSERT_EQ(3U, oldest.timestamp_ms);
    return true;
}
