#include "test_runner.h"

#include <stdio.h>

int test_run_all(const char *suite_name, const test_case_t *tests, size_t test_count)
{
    size_t passed = 0U;

    (void) printf("%s\n", suite_name);

    for (size_t i = 0U; i < test_count; i++)
    {
        const bool result = tests[i].function();

        (void) printf("[%s] %s\n", result ? "PASS" : "FAIL", tests[i].name);

        if (result)
        {
            passed++;
        }
    }

    (void) printf("Summary: %zu/%zu PASS\n", passed, test_count);

    return (passed == test_count) ? 0 : 1;
}
