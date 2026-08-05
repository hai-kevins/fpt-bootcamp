#include "test.h"

#include "cmd_line.h"

#include <stddef.h>
#include <stdint.h>

static uint32_t g_handler_calls;
static uint32_t g_context_value;
static char g_arguments[MAX_ARG_SIZE];

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
    uint32_t *value = (uint32_t *)context;

    g_handler_calls++;
    copy_text(g_arguments, arguments);

    if (value != NULL)
    {
        (*value)++;
    }

    return CMD_SUCCESS;
}

static cmd_line_status_t invalid_handler(
    const char *arguments,
    void *context
)
{
    (void)arguments;
    (void)context;

    return CMD_INVALID_ARG;
}

static const cmd_line_t g_table[] =
{
    {
        "run",
        success_handler,
        "run <arguments>"
    },
    {
        "invalid",
        invalid_handler,
        "invalid"
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
    uint32_t value;

    g_handler_calls = 0U;
    g_context_value = 0U;
    g_arguments[0] = '\0';

    TEST_ASSERT_EQ_INT(
        CMD_SUCCESS,
        cmd_line_parser(
            g_table,
            "  run   alpha beta  \r\n",
            &g_context_value
        )
    );
    TEST_ASSERT_EQ_U32(1U, g_handler_calls);
    TEST_ASSERT_EQ_U32(1U, g_context_value);
    TEST_ASSERT_TRUE(text_equal(g_arguments, "alpha beta"));

    TEST_ASSERT_EQ_INT(
        CMD_SUCCESS,
        cmd_line_parser(g_table, "run", NULL)
    );
    TEST_ASSERT_TRUE(text_equal(g_arguments, ""));

    TEST_ASSERT_EQ_INT(
        CMD_TBL_NOT_FOUND,
        cmd_line_parser(NULL, "run", NULL)
    );
    TEST_ASSERT_EQ_INT(
        CMD_INVALID_INPUT,
        cmd_line_parser(g_table, NULL, NULL)
    );
    TEST_ASSERT_EQ_INT(
        CMD_EMPTY,
        cmd_line_parser(g_table, "  \t\r\n", NULL)
    );
    TEST_ASSERT_EQ_INT(
        CMD_NOT_FOUND,
        cmd_line_parser(g_table, "unknown", NULL)
    );
    TEST_ASSERT_EQ_INT(
        CMD_HANDLER_NOT_FOUND,
        cmd_line_parser(g_table, "missing", NULL)
    );
    TEST_ASSERT_EQ_INT(
        CMD_INVALID_ARG,
        cmd_line_parser(g_table, "invalid", NULL)
    );
    TEST_ASSERT_EQ_INT(
        CMD_TOO_LONG,
        cmd_line_parser(
            g_table,
            "this_command_name_is_far_too_long",
            NULL
        )
    );

    value = 0U;
    TEST_ASSERT_TRUE(cmd_line_parse_u32("4294967295", &value));
    TEST_ASSERT_EQ_U32(UINT32_MAX, value);
    TEST_ASSERT_TRUE(!cmd_line_parse_u32("4294967296", &value));
    TEST_ASSERT_TRUE(!cmd_line_parse_u32("42x", &value));
    TEST_ASSERT_TRUE(cmd_line_parse_u32(" 50 ", &value));
    TEST_ASSERT_EQ_U32(50U, value);

    return true;
}
