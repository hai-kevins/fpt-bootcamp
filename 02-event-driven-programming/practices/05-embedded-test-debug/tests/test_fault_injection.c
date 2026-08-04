#include "fault_injection.h"
#include "test_assert.h"
#include "test_cases.h"

bool test_fault_injection_flags(void)
{
    fault_injection_t faults;

    fault_injection_init(&faults);

    TEST_ASSERT_FALSE(fault_injection_trigger(&faults, FAULT_INJECTION_FLASH_FAILURE));

    fault_injection_enable(&faults, FAULT_INJECTION_FLASH_FAILURE);

    TEST_ASSERT_TRUE(fault_injection_trigger(&faults, FAULT_INJECTION_FLASH_FAILURE));
    TEST_ASSERT_EQ(1U, faults.trigger_count);

    fault_injection_disable(&faults, FAULT_INJECTION_FLASH_FAILURE);

    TEST_ASSERT_FALSE(fault_injection_is_enabled(&faults, FAULT_INJECTION_FLASH_FAILURE));
    return true;
}
