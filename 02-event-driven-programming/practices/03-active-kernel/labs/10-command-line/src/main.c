#include "cmd_line.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>

static unsigned g_posted;

static cmd_line_status_t command_led(
    const char *arguments,
    void *context
)
{
    (void)context;

    if (!cmd_line_argument_equals(arguments, "on") &&
        !cmd_line_argument_equals(arguments, "off"))
    {
        return CMD_INVALID_ARG;
    }

    g_posted++;
    printf("post LED: %s\n", arguments);

    return CMD_SUCCESS;
}

static cmd_line_status_t command_state(
    const char *arguments,
    void *context
)
{
    (void)context;

    if (!cmd_line_argument_is_empty(arguments))
    {
        return CMD_INVALID_ARG;
    }

    g_posted++;
    puts("post STATE request");

    return CMD_SUCCESS;
}

static const cmd_line_t g_command_table[] =
{
    {
        "led",
        command_led,
        "led <on|off>"
    },
    {
        "state",
        command_state,
        "state"
    },
    {
        NULL,
        NULL,
        NULL
    }
};

int main(void)
{
    assert(
        cmd_line_parser(g_command_table, "led on", NULL) ==
        CMD_SUCCESS
    );
    assert(
        cmd_line_parser(g_command_table, "state", NULL) ==
        CMD_SUCCESS
    );
    assert(
        cmd_line_parser(g_command_table, "bad", NULL) ==
        CMD_NOT_FOUND
    );
    assert(
        cmd_line_parser(g_command_table, "led toggle", NULL) ==
        CMD_INVALID_ARG
    );
    assert(g_posted == 2U);

    puts("[PASS] command-table shell");

    return 0;
}
