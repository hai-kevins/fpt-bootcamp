#ifndef TEST_ASSERT_H
#define TEST_ASSERT_H

#include <stdio.h>

#define TEST_ASSERT_TRUE(condition)                                  \
    do                                                               \
    {                                                                \
        if (!(condition))                                            \
        {                                                            \
            (void)fprintf(                                           \
                stderr,                                              \
                "Assertion failed: %s (%s:%d)\n",                    \
                #condition,                                          \
                __FILE__,                                            \
                __LINE__                                             \
            );                                                       \
            return false;                                            \
        }                                                            \
    } while (0)

#define TEST_ASSERT_FALSE(condition) \
    TEST_ASSERT_TRUE(!(condition))

#define TEST_ASSERT_EQ(expected, actual) \
    TEST_ASSERT_TRUE((expected) == (actual))

#define TEST_ASSERT_NE(expected, actual) \
    TEST_ASSERT_TRUE((expected) != (actual))

#endif
