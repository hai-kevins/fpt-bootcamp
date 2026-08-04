#include "test.h"

int test_coalesce_all_blocks(void);
int test_allocate_after_coalesce(void);

int main(void)
{
    TEST_RUN(test_coalesce_all_blocks);
    TEST_RUN(test_allocate_after_coalesce);
    return 0;
}
