#ifndef TEST_RUNNER_H
#define TEST_RUNNER_H

#include <stdbool.h>
#include <stddef.h>

typedef bool (*test_function_t)(void);

typedef struct
{
    const char *name;
    test_function_t function;
} test_case_t;

int test_run_all(const char *suite_name, const test_case_t *tests, size_t test_count);

#endif
