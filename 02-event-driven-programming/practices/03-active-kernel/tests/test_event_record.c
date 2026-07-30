#include "test.h"
#include "event_record.h"

bool test_event_record(void)
{
    ak_event_record_init();
    for (uint32_t i = 0U; i < 70U; i++)
    {
        ak_event_record_t record = {.timestamp=i, .signal=(uint16_t)i};
        ak_event_record_write(&record);
    }
    TEST_ASSERT_EQ(AK_EVENT_RECORD_CAPACITY, ak_event_record_count());
    TEST_ASSERT_EQ(6U, ak_event_record_overwritten());
    ak_event_record_t oldest;
    TEST_ASSERT_TRUE(ak_event_record_read(0U, &oldest));
    TEST_ASSERT_EQ(6U, oldest.timestamp);
    return true;
}
