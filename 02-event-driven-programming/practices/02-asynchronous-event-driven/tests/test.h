#ifndef TEST_H
#define TEST_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

extern uint32_t g_test_passed;
extern uint32_t g_test_failed;

#define TEST_ASSERT_TRUE(condition) \
    do \
    { \
        if (!(condition)) \
        { \
            printf("[FAIL] %s:%d: %s\n", \
                   __FILE__, __LINE__, #condition); \
            g_test_failed++; \
            return false; \
        } \
    } while (0)

#define TEST_ASSERT_EQ_U32(expected, actual) \
    TEST_ASSERT_TRUE((uint32_t)(expected) == (uint32_t)(actual))

#define TEST_ASSERT_EQ_INT(expected, actual) \
    TEST_ASSERT_TRUE((int)(expected) == (int)(actual))

typedef bool (*test_function_t)(void);

void test_run(const char *name, test_function_t function);
void test_reset_framework(void);
void test_dispatch_all(void);

void fake_set_button(bool pressed);
bool fake_get_led(void);
void fake_clear_uart(void);
const char *fake_get_uart(void);

#endif
