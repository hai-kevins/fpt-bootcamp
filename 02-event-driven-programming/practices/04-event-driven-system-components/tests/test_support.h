#ifndef TEST_SUPPORT_H
#define TEST_SUPPORT_H

#include <stdbool.h>
#include <stdio.h>

#define TEST_ASSERT(condition)                                      \
    do                                                              \
    {                                                               \
        if (!(condition))                                           \
        {                                                           \
            (void)printf("    assertion failed: %s (%s:%d)\n",     \
                         #condition, __FILE__, __LINE__);            \
            return false;                                           \
        }                                                           \
    } while (0)

typedef bool (*test_function_t)(void);

typedef struct
{
    const char *name;
    test_function_t function;
} test_case_t;

#endif
