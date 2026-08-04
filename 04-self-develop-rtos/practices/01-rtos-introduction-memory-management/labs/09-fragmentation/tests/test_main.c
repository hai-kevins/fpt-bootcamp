#include "test.h"

int test_statistics_and_fragmentation(void);
int test_randomized_allocator_sequence(void);

int main(void)
{
    TEST_RUN(test_statistics_and_fragmentation);
    TEST_RUN(test_randomized_allocator_sequence);
    return 0;
}
