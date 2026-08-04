#include "test.h"
#include "stack_monitor.h"
#include <stdint.h>

int test_stack_monitor_empty(void)
{
    uint8_t memory[64];
    stack_monitor_fill(memory, sizeof(memory), 0xA5U);
    TEST_ASSERT(stack_monitor_high_water(memory, sizeof(memory), 0xA5U, true) == 0U);
    return 0;
}

int test_stack_monitor_downward(void)
{
    uint8_t memory[64];
    size_t index;
    stack_monitor_fill(memory, sizeof(memory), 0xA5U);

    for (index = 0U; index < 17U; index++)
    {
        memory[sizeof(memory) - 1U - index] = 0U;
    }

    TEST_ASSERT(stack_monitor_high_water(memory, sizeof(memory), 0xA5U, true) == 17U);
    return 0;
}

int test_stack_monitor_upward(void)
{
    uint8_t memory[64];
    size_t index;
    stack_monitor_fill(memory, sizeof(memory), 0xA5U);

    for (index = 0U; index < 12U; index++)
    {
        memory[index] = 0U;
    }

    TEST_ASSERT(stack_monitor_high_water(memory, sizeof(memory), 0xA5U, false) == 12U);
    return 0;
}
