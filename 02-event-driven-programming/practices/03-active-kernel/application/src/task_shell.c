#include "task_shell.h"

#include "app.h"
#include "app_signals.h"
#include "app_tasks.h"
#include "cmd_line.h"
#include "event_record.h"
#include "fatal.h"
#include "message.h"
#include "task.h"
#include "timer.h"
#include "uart.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define SHELL_LINE_CAPACITY (48U)

static char g_line[SHELL_LINE_CAPACITY];
static uint8_t g_length;
static bool g_overflow;

static cmd_line_status_t command_help(
    const char *arguments,
    void *context
);
static cmd_line_status_t command_task(
    const char *arguments,
    void *context
);
static cmd_line_status_t command_pool(
    const char *arguments,
    void *context
);
static cmd_line_status_t command_timer(
    const char *arguments,
    void *context
);
static cmd_line_status_t command_state(
    const char *arguments,
    void *context
);
static cmd_line_status_t command_led(
    const char *arguments,
    void *context
);
static cmd_line_status_t command_event(
    const char *arguments,
    void *context
);
static cmd_line_status_t command_fatal(
    const char *arguments,
    void *context
);
static cmd_line_status_t command_error(
    const char *arguments,
    void *context
);
static cmd_line_status_t command_reset(
    const char *arguments,
    void *context
);

static const cmd_line_t g_command_table[] =
{
    {
        "help",
        command_help,
        "help                       - show this command list"
    },
    {
        "task",
        command_task,
        "task                       - show task statistics"
    },
    {
        "pool",
        command_pool,
        "pool                       - show message-pool statistics"
    },
    {
        "timer",
        command_timer,
        "timer                      - show timer statistics"
    },
    {
        "state",
        command_state,
        "state                      - show application state"
    },
    {
        "led",
        command_led,
        "led <on|off|toggle>         - control the LED task"
    },
    {
        "event",
        command_event,
        "event <dump|clear>          - inspect event records"
    },
    {
        "fatal",
        command_fatal,
        "fatal <show|test|clear>     - inspect fatal records"
    },
    {
        "error",
        command_error,
        "error                      - post an application error"
    },
    {
        "reset",
        command_reset,
        "reset                      - post an application reset"
    },
    {
        NULL,
        NULL,
        NULL
    }
};

static bool no_arguments(const char *arguments)
{
    return cmd_line_argument_is_empty(arguments);
}

static void write_u32(uint32_t value)
{
    char digits[11];
    uint8_t count = 0U;

    if (value == 0U)
    {
        uart_write_string("0");
        return;
    }

    while ((value > 0U) && (count < sizeof(digits)))
    {
        digits[count] = (char)('0' + (value % 10U));
        count++;
        value /= 10U;
    }

    while (count > 0U)
    {
        count--;
        uart_write_byte((uint8_t)digits[count]);
    }
}

static cmd_line_status_t post_pure(
    uint8_t destination,
    uint16_t signal
)
{
    ak_message_t *message = ak_message_create_pure(
        APP_TASK_SHELL,
        destination,
        signal
    );

    if (message == NULL)
    {
        return CMD_EXECUTION_ERROR;
    }

    if (!ak_task_post(message))
    {
        ak_message_release(message);
        return CMD_EXECUTION_ERROR;
    }

    return CMD_SUCCESS;
}

static cmd_line_status_t command_help(
    const char *arguments,
    void *context
)
{
    size_t index = 0U;

    (void)context;

    if (!no_arguments(arguments))
    {
        return CMD_INVALID_ARG;
    }

    while (g_command_table[index].cmd != NULL)
    {
        uart_write_string(g_command_table[index].info);
        uart_write_string("\r\n");
        index++;
    }

    return CMD_SUCCESS;
}

static cmd_line_status_t command_task(
    const char *arguments,
    void *context
)
{
    uint8_t id;

    (void)context;

    if (!no_arguments(arguments))
    {
        return CMD_INVALID_ARG;
    }

    for (id = 1U; id <= 5U; id++)
    {
        ak_task_stats_t stats;

        if (ak_task_get_stats(id, &stats))
        {
            uart_write_string(stats.name);
            uart_write_string(" q=");
            write_u32(stats.queue_depth);
            uart_write_string(" max=");
            write_u32(stats.queue_depth_max);
            uart_write_string(" drop=");
            write_u32(stats.dropped);
            uart_write_string("\r\n");
        }
    }

    return CMD_SUCCESS;
}

static cmd_line_status_t command_pool(
    const char *arguments,
    void *context
)
{
    ak_message_pool_stats_t stats;

    (void)context;

    if (!no_arguments(arguments))
    {
        return CMD_INVALID_ARG;
    }

    stats = ak_message_pool_stats();
    uart_write_string("pool used=");
    write_u32(stats.used);
    uart_write_string(" max=");
    write_u32(stats.used_max);
    uart_write_string(" fail=");
    write_u32(stats.allocation_failures);
    uart_write_string("\r\n");

    return CMD_SUCCESS;
}

static cmd_line_status_t command_timer(
    const char *arguments,
    void *context
)
{
    ak_timer_stats_t stats;

    (void)context;

    if (!no_arguments(arguments))
    {
        return CMD_INVALID_ARG;
    }

    stats = ak_timer_stats();
    uart_write_string("timer active=");
    write_u32(stats.active_count);
    uart_write_string(" max=");
    write_u32(stats.active_max);
    uart_write_string(" expire=");
    write_u32(stats.expirations);
    uart_write_string("\r\n");

    return CMD_SUCCESS;
}

static cmd_line_status_t command_state(
    const char *arguments,
    void *context
)
{
    (void)context;

    if (!no_arguments(arguments))
    {
        return CMD_INVALID_ARG;
    }

    uart_write_string("app state=");
    write_u32(app_state_get());
    uart_write_string("\r\n");

    return CMD_SUCCESS;
}

static cmd_line_status_t command_led(
    const char *arguments,
    void *context
)
{
    (void)context;

    if (cmd_line_argument_equals(arguments, "on"))
    {
        return post_pure(APP_TASK_LED, APP_SIGNAL_LED_ON);
    }

    if (cmd_line_argument_equals(arguments, "off"))
    {
        return post_pure(APP_TASK_LED, APP_SIGNAL_LED_OFF);
    }

    if (cmd_line_argument_equals(arguments, "toggle"))
    {
        return post_pure(APP_TASK_LED, APP_SIGNAL_LED_TOGGLE);
    }

    return CMD_INVALID_ARG;
}

static cmd_line_status_t command_event(
    const char *arguments,
    void *context
)
{
    (void)context;

    if (cmd_line_argument_equals(arguments, "clear"))
    {
        ak_event_record_clear();
        uart_write_string("event record cleared\r\n");
        return CMD_SUCCESS;
    }

    if (cmd_line_argument_equals(arguments, "dump"))
    {
        uart_write_string("event count=");
        write_u32((uint32_t)ak_event_record_count());
        uart_write_string(" overwritten=");
        write_u32(ak_event_record_overwritten());
        uart_write_string("\r\n");
        return CMD_SUCCESS;
    }

    return CMD_INVALID_ARG;
}

static cmd_line_status_t command_fatal(
    const char *arguments,
    void *context
)
{
    (void)context;

    if (cmd_line_argument_equals(arguments, "show"))
    {
        if (ak_fatal_record_valid())
        {
            const ak_fatal_record_t record = ak_fatal_record_get();

            uart_write_string("fatal code=");
            write_u32(record.error_code);
            uart_write_string(" task=");
            write_u32(record.task_id);
            uart_write_string(" restart=");
            write_u32(record.restart_count);
            uart_write_string("\r\n");
        }
        else
        {
            uart_write_string("no fatal record\r\n");
        }

        return CMD_SUCCESS;
    }

    if (cmd_line_argument_equals(arguments, "clear"))
    {
        ak_fatal_clear();
        uart_write_string("fatal cleared\r\n");
        return CMD_SUCCESS;
    }

    if (cmd_line_argument_equals(arguments, "test"))
    {
        ak_fatal_raise(
            1001U,
            APP_TASK_SHELL,
            APP_SIGNAL_SHELL_COMMAND
        );
        return CMD_SUCCESS;
    }

    return CMD_INVALID_ARG;
}

static cmd_line_status_t command_error(
    const char *arguments,
    void *context
)
{
    (void)context;

    if (!no_arguments(arguments))
    {
        return CMD_INVALID_ARG;
    }

    return post_pure(
        APP_TASK_APPLICATION,
        APP_SIGNAL_APP_ERROR
    );
}

static cmd_line_status_t command_reset(
    const char *arguments,
    void *context
)
{
    (void)context;

    if (!no_arguments(arguments))
    {
        return CMD_INVALID_ARG;
    }

    return post_pure(
        APP_TASK_APPLICATION,
        APP_SIGNAL_APP_RESET
    );
}

static void report_command_status(cmd_line_status_t status)
{
    switch (status)
    {
        case CMD_SUCCESS:
        case CMD_EMPTY:
        {
            break;
        }

        case CMD_NOT_FOUND:
        {
            uart_write_string("unknown command\r\n");
            break;
        }

        case CMD_TOO_LONG:
        {
            uart_write_string("command too long\r\n");
            break;
        }

        case CMD_INVALID_ARG:
        {
            uart_write_string("invalid argument\r\n");
            break;
        }

        case CMD_EXECUTION_ERROR:
        {
            uart_write_string("command failed\r\n");
            break;
        }

        default:
        {
            uart_write_string("parser failure\r\n");
            break;
        }
    }
}

static void execute_line(void)
{
    const cmd_line_status_t status = cmd_line_parser(
        g_command_table,
        g_line,
        NULL
    );

    report_command_status(status);
}

void task_shell_init(void)
{
    g_length = 0U;
    g_overflow = false;
    g_line[0] = '\0';
}

void task_shell_handler(const ak_message_t *message)
{
    uint8_t byte;

    if ((message == NULL) ||
        (message->signal != APP_SIGNAL_UART_RX_BYTE) ||
        (message->length != 1U))
    {
        return;
    }

    byte = message->payload[0];

    if ((byte == (uint8_t)'\r') ||
        (byte == (uint8_t)'\n'))
    {
        if (g_overflow)
        {
            uart_write_string("command too long\r\n");
        }
        else if (g_length > 0U)
        {
            g_line[g_length] = '\0';
            execute_line();
        }

        g_length = 0U;
        g_overflow = false;
        g_line[0] = '\0';
    }
    else if ((byte == 0x08U) || (byte == 0x7FU))
    {
        if (!g_overflow && (g_length > 0U))
        {
            g_length--;
        }
    }
    else if (!g_overflow &&
             (g_length < (SHELL_LINE_CAPACITY - 1U)))
    {
        g_line[g_length] = (char)byte;
        g_length++;
    }
    else
    {
        g_overflow = true;
    }
}
