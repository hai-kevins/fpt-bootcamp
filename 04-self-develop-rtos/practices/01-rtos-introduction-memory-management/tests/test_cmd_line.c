#include "cmd_line.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

static uint32_t g_handler_calls;
static char g_received_arguments[MAX_ARG_SIZE];

#define TEST_ASSERT(condition) \
    do \
    { \
        if (!(condition)) \
        { \
            printf( \
                "FAIL %s:%d: %s\n", \
                __FILE__, \
                __LINE__, \
                #condition \
            ); \
            return false; \
        } \
    } while (0)

static void copy_text(char *destination, const char *source)
{
    size_t index = 0U;

    while ((source[index] != '\0') &&
           ((index + 1U) < MAX_ARG_SIZE))
    {
        destination[index] = source[index];
        index++;
    }

    destination[index] = '\0';
}

static bool text_equal(const char *left, const char *right)
{
    while ((*left != '\0') && (*right != '\0'))
    {
        if (*left != *right)
        {
            return false;
        }

        left++;
        right++;
    }

    return (*left == '\0') && (*right == '\0');
}

static cmd_line_status_t test_handler(
    const char *arguments,
    void *context
)
{
    uint32_t *context_value = (uint32_t *)context;

    g_handler_calls++;
    copy_text(g_received_arguments, arguments);

    if (context_value != NULL)
    {
        (*context_value)++;
    }

    return CMD_SUCCESS;
}

static cmd_line_status_t error_handler(
    const char *arguments,
    void *context
)
{
    (void)arguments;
    (void)context;

    return CMD_EXECUTION_ERROR;
}

static const cmd_line_t g_test_table[] =
{
    {
        "help",
        test_handler,
        "help"
    },
    {
        "error",
        error_handler,
        "error"
    },
    {
        "missing",
        NULL,
        "missing"
    },
    {
        NULL,
        NULL,
        NULL
    }
};

static bool test_parser_dispatch_and_trim(void)
{
    uint32_t context_value = 0U;
    cmd_line_status_t status;

    g_handler_calls = 0U;
    g_received_arguments[0] = '\0';

    status = cmd_line_parser(
        g_test_table,
        "  help   alpha beta  \r\n",
        &context_value
    );

    TEST_ASSERT(status == CMD_SUCCESS);
    TEST_ASSERT(g_handler_calls == 1U);
    TEST_ASSERT(context_value == 1U);
    TEST_ASSERT(text_equal(g_received_arguments, "alpha beta"));

    return true;
}

static bool test_parser_statuses(void)
{
    TEST_ASSERT(
        cmd_line_parser(NULL, "help", NULL) ==
        CMD_TBL_NOT_FOUND
    );
    TEST_ASSERT(
        cmd_line_parser(g_test_table, NULL, NULL) ==
        CMD_INVALID_INPUT
    );
    TEST_ASSERT(
        cmd_line_parser(g_test_table, " \t\r\n", NULL) ==
        CMD_EMPTY
    );
    TEST_ASSERT(
        cmd_line_parser(g_test_table, "unknown", NULL) ==
        CMD_NOT_FOUND
    );
    TEST_ASSERT(
        cmd_line_parser(g_test_table, "missing", NULL) ==
        CMD_HANDLER_NOT_FOUND
    );
    TEST_ASSERT(
        cmd_line_parser(g_test_table, "error", NULL) ==
        CMD_EXECUTION_ERROR
    );

    return true;
}

static bool test_parser_without_newline(void)
{
    g_handler_calls = 0U;

    TEST_ASSERT(
        cmd_line_parser(g_test_table, "help", NULL) ==
        CMD_SUCCESS
    );
    TEST_ASSERT(g_handler_calls == 1U);
    TEST_ASSERT(text_equal(g_received_arguments, ""));

    return true;
}

static bool test_parser_rejects_long_command(void)
{
    const char command[] =
        "this_command_name_is_far_too_long_for_the_parser";

    TEST_ASSERT(
        cmd_line_parser(g_test_table, command, NULL) ==
        CMD_TOO_LONG
    );

    return true;
}

static bool test_parse_u32(void)
{
    uint32_t value = 0U;

    TEST_ASSERT(cmd_line_parse_u32("4294967295", &value));
    TEST_ASSERT(value == UINT32_MAX);
    TEST_ASSERT(!cmd_line_parse_u32("4294967296", &value));
    TEST_ASSERT(!cmd_line_parse_u32("12x", &value));
    TEST_ASSERT(!cmd_line_parse_u32("", &value));
    TEST_ASSERT(cmd_line_parse_u32(" 42 ", &value));
    TEST_ASSERT(value == 42U);

    return true;
}

int main(void)
{
    const bool passed =
        test_parser_dispatch_and_trim() &&
        test_parser_statuses() &&
        test_parser_without_newline() &&
        test_parser_rejects_long_command() &&
        test_parse_u32();

    printf(
        "%s: cmd_line tests\n",
        passed ? "PASS" : "FAIL"
    );

    return passed ? 0 : 1;
}
