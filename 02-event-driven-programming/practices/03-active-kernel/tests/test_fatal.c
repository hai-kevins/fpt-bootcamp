#include "test.h"
#include "fatal.h"

extern uint32_t g_fake_reset_count;
extern uint32_t g_fake_time;

bool test_fatal_record(void)
{
    ak_fatal_clear();
    g_fake_time = 123U;
    g_fake_reset_count = 0U;
    ak_fatal_capture(55U, 3U, 9U);
    TEST_ASSERT_TRUE(ak_fatal_record_valid());
    ak_fatal_record_t record = ak_fatal_record_get();
    TEST_ASSERT_EQ(55U, record.error_code);
    TEST_ASSERT_EQ(3U, record.task_id);
    TEST_ASSERT_EQ(123U, record.timestamp);
    ak_fatal_raise(77U, 2U, 8U);
    TEST_ASSERT_EQ(1U, g_fake_reset_count);
    return true;
}
