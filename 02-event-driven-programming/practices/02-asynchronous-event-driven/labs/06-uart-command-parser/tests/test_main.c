#include "cmd_line.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

static uint32_t g_led_on_count;
static uint32_t g_led_off_count;
static uint32_t g_blink_period;
static uint32_t g_status_count;

static cmd_line_status_t command_led(
    const char *arguments,
    void *context
)
{
    (void)context;

    if (cmd_line_argument_equals(arguments, "on"))
    {
        g_led_on_count++;
        return CMD_SUCCESS;
    }

    if (cmd_line_argument_equals(arguments, "off"))
    {
        g_led_off_count++;
        return CMD_SUCCESS;
    }

    return CMD_INVALID_ARG;
}

static cmd_line_status_t command_blink(
    const char *arguments,
    void *context
)
{
    uint32_t period_ms;

    (void)context;

    if (!cmd_line_parse_u32(arguments, &period_ms) ||
        (period_ms < 50U) ||
        (period_ms > 5000U))
    {
        return CMD_INVALID_ARG;
    }

    g_blink_period = period_ms;

    return CMD_SUCCESS;
}

static cmd_line_status_t command_status(
    const char *arguments,
    void *context
)
{
    (void)context;

    if (!cmd_line_argument_is_empty(arguments))
    {
        return CMD_INVALID_ARG;
    }

    g_status_count++;

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
        "blink",
        command_blink,
        "blink <50..5000>"
    },
    {
        "status",
        command_status,
        "status"
    },
    {
        NULL,
        NULL,
        NULL
    }
};

int main(void)
{
    bool pass = true;

    pass = pass &&
        (cmd_line_parser(g_command_table, "led on", NULL) ==
         CMD_SUCCESS);
    pass = pass &&
        (cmd_line_parser(g_command_table, "led off", NULL) ==
         CMD_SUCCESS);
    pass = pass &&
        (cmd_line_parser(g_command_table, "blink 500", NULL) ==
         CMD_SUCCESS);
    pass = pass &&
        (cmd_line_parser(g_command_table, "status", NULL) ==
         CMD_SUCCESS);
    pass = pass &&
        (cmd_line_parser(g_command_table, "blink 1", NULL) ==
         CMD_INVALID_ARG);
    pass = pass &&
        (cmd_line_parser(g_command_table, "unknown", NULL) ==
         CMD_NOT_FOUND);
    pass = pass &&
        (cmd_line_parser(g_command_table, "led", NULL) ==
         CMD_INVALID_ARG);
    pass = pass &&
        (cmd_line_parser(g_command_table, "  led   on  ", NULL) ==
         CMD_SUCCESS);
    pass = pass && (g_led_on_count == 2U);
    pass = pass && (g_led_off_count == 1U);
    pass = pass && (g_blink_period == 500U);
    pass = pass && (g_status_count == 1U);

    printf(
        "[%s] command-table-parser\n",
        pass ? "PASS" : "FAIL"
    );

    return pass ? 0 : 1;
}
