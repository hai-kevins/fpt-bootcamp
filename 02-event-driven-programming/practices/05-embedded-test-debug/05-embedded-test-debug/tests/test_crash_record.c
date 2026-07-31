#include "crash_record.h"
#include "test_assert.h"
#include "test_cases.h"

bool test_crash_record_validation(void)
{
    crash_record_t record;

    crash_record_prepare(&record);
    record.build_id = 123U;
    record.fatal_code = 0xE001U;
    crash_record_finalize(&record);

    TEST_ASSERT_TRUE(crash_record_is_valid(&record));

    record.fatal_code ^= 1U;
    TEST_ASSERT_FALSE(crash_record_is_valid(&record));
    return true;
}
