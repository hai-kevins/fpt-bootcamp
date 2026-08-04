#include "test.h"

int test_invalid_free_foreign_pointer(void);
int test_invalid_free_middle_pointer(void);
int test_double_free(void);

int main(void)
{
    TEST_RUN(test_invalid_free_foreign_pointer);
    TEST_RUN(test_invalid_free_middle_pointer);
    TEST_RUN(test_double_free);
    return 0;
}
