#include "test.h"

int test_split_allows_multiple_allocations(void);
int test_free_does_not_coalesce_yet(void);

int main(void)
{
    TEST_RUN(test_split_allows_multiple_allocations);
    TEST_RUN(test_free_does_not_coalesce_yet);
    return 0;
}
