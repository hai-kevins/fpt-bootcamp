#include "test.h"

#include "cmd_line.h"

#include <stddef.h>
#include <stdint.h>

static uint32_t g_handler_calls;
static char g_received_arguments[MAX_ARG_SIZE];

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

static cmd_line_status_t success_handler(
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

static cmd_line_status_t execution_error_handler(
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
        "run",
        success_handler,
        "run <arguments>"
    },
    {
        "error",
        execution_error_handler,
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

bool test_cmd_line_parser(void)
{
    uint32_t context_value = 0U;
    uint32_t parsed_value = 0U;

    g_handler_calls = 0U;
    g_received_arguments[0] = '\0';

    TEST_ASSERT_EQ(
        CMD_SUCCESS,
        cmd_line_parser(
            g_test_table,
            "  run   alpha beta  \r\n",
            &context_value
        )
    );
    TEST_ASSERT_EQ(1U, g_handler_calls);
    TEST_ASSERT_EQ(1U, context_value);
    TEST_ASSERT_TRUE(
        text_equal(g_received_arguments, "alpha beta")
    );

    TEST_ASSERT_EQ(
        CMD_SUCCESS,
        cmd_line_parser(g_test_table, "run", NULL)
    );
    TEST_ASSERT_TRUE(text_equal(g_received_arguments, ""));

    TEST_ASSERT_EQ(
        CMD_TBL_NOT_FOUND,
        cmd_line_parser(NULL, "run", NULL)
    );
    TEST_ASSERT_EQ(
        CMD_INVALID_INPUT,
        cmd_line_parser(g_test_table, NULL, NULL)
    );
    TEST_ASSERT_EQ(
        CMD_EMPTY,
        cmd_line_parser(g_test_table, " \t\r\n", NULL)
    );
    TEST_ASSERT_EQ(
        CMD_NOT_FOUND,
        cmd_line_parser(g_test_table, "unknown", NULL)
    );
    TEST_ASSERT_EQ(
        CMD_HANDLER_NOT_FOUND,
        cmd_line_parser(g_test_table, "missing", NULL)
    );
    TEST_ASSERT_EQ(
        CMD_EXECUTION_ERROR,
        cmd_line_parser(g_test_table, "error", NULL)
    );
    TEST_ASSERT_EQ(
        CMD_TOO_LONG,
        cmd_line_parser(
            g_test_table,
            "this_command_name_is_far_too_long",
            NULL
        )
    );

    TEST_ASSERT_TRUE(
        cmd_line_parse_u32("4294967295", &parsed_value)
    );
    TEST_ASSERT_EQ(UINT32_MAX, parsed_value);
    TEST_ASSERT_TRUE(
        !cmd_line_parse_u32("4294967296", &parsed_value)
    );
    TEST_ASSERT_TRUE(
        !cmd_line_parse_u32("12x", &parsed_value)
    );

    return true;
}
