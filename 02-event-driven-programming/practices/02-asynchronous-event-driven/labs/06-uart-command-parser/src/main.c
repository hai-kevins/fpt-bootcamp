#include "cmd_line.h"

#include <stddef.h>
#include <stdio.h>

static cmd_line_status_t command_led(
    const char *arguments,
    void *context
)
{
    (void)context;

    if (cmd_line_argument_equals(arguments, "on") ||
        cmd_line_argument_equals(arguments, "off"))
    {
        printf("LED: %s\n", arguments);
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

    printf("BLINK: %lu ms\n", (unsigned long)period_ms);

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

    puts("STATUS");

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
    static const char *commands[] =
    {
        "led on",
        "led off",
        "blink 500",
        "status",
        "blink abc"
    };
    size_t index;

    for (index = 0U;
         index < (sizeof(commands) / sizeof(commands[0]));
         index++)
    {
        const cmd_line_status_t status = cmd_line_parser(
            g_command_table,
            commands[index],
            NULL
        );

        printf(
            "%s -> status=%d\n",
            commands[index],
            (int)status
        );
    }

    return 0;
}
