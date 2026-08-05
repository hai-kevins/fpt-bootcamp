#include "uart_service.h"

#include "cmd_line.h"
#include "dispatcher.h"
#include "event_trace.h"
#include "platform.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define UART_COMMAND_CAPACITY (32U)

typedef struct
{
    uint32_t timestamp_ms;
} command_context_t;

static char g_command[UART_COMMAND_CAPACITY];
static size_t g_length;

static cmd_line_status_t command_help(
    const char *arguments,
    void *context
);
static cmd_line_status_t command_led(
    const char *arguments,
    void *context
);
static cmd_line_status_t command_blink(
    const char *arguments,
    void *context
);
static cmd_line_status_t command_status(
    const char *arguments,
    void *context
);
static cmd_line_status_t command_trace(
    const char *arguments,
    void *context
);
static cmd_line_status_t command_reset(
    const char *arguments,
    void *context
);
static cmd_line_status_t command_error(
    const char *arguments,
    void *context
);

static const cmd_line_t g_command_table[] =
{
    {
        "help",
        command_help,
        "help                 - show this command list"
    },
    {
        "led",
        command_led,
        "led <on|off>         - control the LED"
    },
    {
        "blink",
        command_blink,
        "blink <50..5000|stop> - start or stop LED blinking"
    },
    {
        "status",
        command_status,
        "status               - request application state"
    },
    {
        "trace",
        command_trace,
        "trace                - dump the event trace as CSV"
    },
    {
        "reset",
        command_reset,
        "reset                - request application reset"
    },
    {
        "error",
        command_error,
        "error                - inject an application error"
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
    static const uint32_t divisors[] =
    {
        1000000000U,
        100000000U,
        10000000U,
        1000000U,
        100000U,
        10000U,
        1000U,
        100U,
        10U,
        1U
    };

    bool started = false;
    size_t index;

    for (index = 0U;
         index < (sizeof(divisors) / sizeof(divisors[0]));
         index++)
    {
        uint8_t digit = 0U;

        while (value >= divisors[index])
        {
            value -= divisors[index];
            digit++;
        }

        if ((digit != 0U) || started || (divisors[index] == 1U))
        {
            started = true;
            platform_uart_write_byte(
                (uint8_t)((uint8_t)'0' + digit)
            );
        }
    }
}

static void write_csv_record(const event_trace_record_t *record)
{
    if (record == NULL)
    {
        return;
    }

    write_u32(record->event.timestamp_ms);
    platform_uart_write_byte((uint8_t)',');
    write_u32((uint32_t)record->event.source);
    platform_uart_write_byte((uint8_t)',');
    write_u32((uint32_t)record->event.destination);
    platform_uart_write_byte((uint8_t)',');
    write_u32((uint32_t)record->event.signal);
    platform_uart_write_byte((uint8_t)',');
    write_u32(record->event.parameter);
    platform_uart_write_byte((uint8_t)',');
    write_u32((uint32_t)record->phase);
    platform_uart_write_string("\r\n");
}

static void dump_trace(void)
{
    event_trace_record_t record;
    const size_t count = event_trace_count();
    size_t index;

    platform_uart_write_string(
        "timestamp,source,destination,signal,parameter,phase\r\n"
    );

    for (index = 0U; index < count; index++)
    {
        if (event_trace_get(index, &record))
        {
            write_csv_record(&record);
        }
    }
}

static cmd_line_status_t post_command_event(
    const command_context_t *command_context,
    event_destination_t destination,
    signal_t signal,
    uint32_t parameter
)
{
    if (command_context == NULL)
    {
        return CMD_EXECUTION_ERROR;
    }

    if (!dispatcher_post(
            EVENT_SOURCE_UART,
            destination,
            signal,
            parameter,
            command_context->timestamp_ms
        ))
    {
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
        platform_uart_write_string(g_command_table[index].info);
        platform_uart_write_string("\r\n");
        index++;
    }

    return CMD_SUCCESS;
}

static cmd_line_status_t command_led(
    const char *arguments,
    void *context
)
{
    const command_context_t *command_context =
        (const command_context_t *)context;

    if (cmd_line_argument_equals(arguments, "on"))
    {
        return post_command_event(
            command_context,
            EVENT_DESTINATION_LED,
            SIGNAL_LED_ON,
            0U
        );
    }

    if (cmd_line_argument_equals(arguments, "off"))
    {
        return post_command_event(
            command_context,
            EVENT_DESTINATION_LED,
            SIGNAL_LED_OFF,
            0U
        );
    }

    return CMD_INVALID_ARG;
}

static cmd_line_status_t command_blink(
    const char *arguments,
    void *context
)
{
    const command_context_t *command_context =
        (const command_context_t *)context;
    uint32_t blink_period_ms;

    if (cmd_line_argument_equals(arguments, "stop"))
    {
        return post_command_event(
            command_context,
            EVENT_DESTINATION_LED,
            SIGNAL_LED_BLINK_STOP,
            0U
        );
    }

    if (!cmd_line_parse_u32(arguments, &blink_period_ms) ||
        (blink_period_ms < 50U) ||
        (blink_period_ms > 5000U))
    {
        return CMD_INVALID_ARG;
    }

    return post_command_event(
        command_context,
        EVENT_DESTINATION_LED,
        SIGNAL_LED_BLINK_START,
        blink_period_ms
    );
}

static cmd_line_status_t command_status(
    const char *arguments,
    void *context
)
{
    if (!no_arguments(arguments))
    {
        return CMD_INVALID_ARG;
    }

    return post_command_event(
        (const command_context_t *)context,
        EVENT_DESTINATION_APP,
        SIGNAL_UART_STATUS_REQUEST,
        0U
    );
}

static cmd_line_status_t command_trace(
    const char *arguments,
    void *context
)
{
    (void)context;

    if (!no_arguments(arguments))
    {
        return CMD_INVALID_ARG;
    }

    dump_trace();

    return CMD_SUCCESS;
}

static cmd_line_status_t command_reset(
    const char *arguments,
    void *context
)
{
    if (!no_arguments(arguments))
    {
        return CMD_INVALID_ARG;
    }

    return post_command_event(
        (const command_context_t *)context,
        EVENT_DESTINATION_APP,
        SIGNAL_RESET_REQUEST,
        0U
    );
}

static cmd_line_status_t command_error(
    const char *arguments,
    void *context
)
{
    if (!no_arguments(arguments))
    {
        return CMD_INVALID_ARG;
    }

    return post_command_event(
        (const command_context_t *)context,
        EVENT_DESTINATION_APP,
        SIGNAL_ERROR_DETECTED,
        0U
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
            platform_uart_write_string("ERR: unknown command\r\n");
            break;
        }

        case CMD_TOO_LONG:
        {
            platform_uart_write_string("ERR: command too long\r\n");
            break;
        }

        case CMD_INVALID_ARG:
        {
            platform_uart_write_string("ERR: invalid argument\r\n");
            break;
        }

        case CMD_EXECUTION_ERROR:
        {
            platform_uart_write_string("ERR: command failed\r\n");
            break;
        }

        default:
        {
            platform_uart_write_string("ERR: parser failure\r\n");
            break;
        }
    }
}

static void execute_command(uint32_t timestamp_ms)
{
    command_context_t context;
    cmd_line_status_t status;

    context.timestamp_ms = timestamp_ms;
    status = cmd_line_parser(
        g_command_table,
        g_command,
        &context
    );

    report_command_status(status);
}

void uart_service_init(void)
{
    g_length = 0U;
    g_command[0] = '\0';
}

void uart_service_event_handler(const event_t *event)
{
    uint8_t byte;

    if (event == NULL)
    {
        return;
    }

    switch ((signal_t)event->signal)
    {
        case SIGNAL_UART_RX_BYTE:
        {
            byte = (uint8_t)(event->parameter & 0xFFU);

            if (byte == (uint8_t)'\r')
            {
                break;
            }

            if (byte == (uint8_t)'\n')
            {
                g_command[g_length] = '\0';
                execute_command(event->timestamp_ms);
                g_length = 0U;
                g_command[0] = '\0';
                platform_uart_write_string("> ");
                break;
            }

            if (g_length < (UART_COMMAND_CAPACITY - 1U))
            {
                g_command[g_length] = (char)byte;
                g_length++;
            }
            else
            {
                g_length = 0U;
                g_command[0] = '\0';
                platform_uart_write_string(
                    "\r\nERR: command too long\r\n> "
                );
            }

            break;
        }

        case SIGNAL_UART_STATUS_BOOT:
        {
            platform_uart_write_string("APP state: BOOT\r\n");
            break;
        }

        case SIGNAL_UART_STATUS_IDLE:
        {
            platform_uart_write_string("APP state: IDLE\r\n");
            break;
        }

        case SIGNAL_UART_STATUS_ACTIVE:
        {
            platform_uart_write_string("APP state: ACTIVE\r\n");
            break;
        }

        case SIGNAL_UART_STATUS_ERROR:
        {
            platform_uart_write_string("APP state: ERROR\r\n");
            break;
        }

        default:
        {
            break;
        }
    }
}

size_t uart_service_command_length(void)
{
    return g_length;
}
