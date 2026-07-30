#ifndef TEST_H
#define TEST_H

#include <stdbool.h>
#include <stdio.h>

#define TEST_ASSERT_TRUE(condition) \
    do { if (!(condition)) { \
        printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #condition); \
        return false; \
    } } while (0)

#define TEST_ASSERT_EQ(expected, actual) \
    TEST_ASSERT_TRUE((expected) == (actual))

#endif
