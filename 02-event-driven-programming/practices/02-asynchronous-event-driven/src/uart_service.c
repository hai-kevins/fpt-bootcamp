#include "uart_service.h"

#include "dispatcher.h"
#include "event_trace.h"
#include "platform.h"

#include <stdbool.h>
#include <stdint.h>

#define UART_COMMAND_CAPACITY (32U)

static char g_command[UART_COMMAND_CAPACITY];
static size_t g_length;

static bool string_equal(const char *left, const char *right)
{
    if ((left == NULL) || (right == NULL))
    {
        return false;
    }

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

static bool string_starts_with(const char *text, const char *prefix)
{
    if ((text == NULL) || (prefix == NULL))
    {
        return false;
    }

    while (*prefix != '\0')
    {
        if (*text != *prefix)
        {
            return false;
        }

        text++;
        prefix++;
    }

    return true;
}

static bool parse_u32(const char *text, uint32_t *value)
{
    uint32_t result = 0UL;
    bool has_digit = false;

    if ((text == NULL) || (value == NULL))
    {
        return false;
    }

    while (*text != '\0')
    {
        uint32_t digit;

        if ((*text < '0') || (*text > '9'))
        {
            return false;
        }

        digit = (uint32_t)(*text - '0');

        if ((result > 429496729U) || ((result == 429496729U) && (digit > 5U)))
        {
            return false;
        }

        result = (result * 10U) + digit;
        has_digit = true;
        text++;
    }

    if (!has_digit)
    {
        return false;
    }

    *value = result;
    return true;
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

    for (size_t i = 0U; i < (sizeof(divisors) / sizeof(divisors[0])); i++)
    {
        uint8_t digit = 0U;

        while (value >= divisors[i])
        {
            value -= divisors[i];
            digit++;
        }

        if ((digit != 0U) || started || (divisors[i] == 1U))
        {
            started = true;
            platform_uart_write_byte((uint8_t)((uint8_t)'0' + digit));
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
    write_u32((uint32_t) record->event.source);
    platform_uart_write_byte((uint8_t)',');
    write_u32((uint32_t) record->event.destination);
    platform_uart_write_byte((uint8_t)',');
    write_u32((uint32_t) record->event.signal);
    platform_uart_write_byte((uint8_t)',');
    write_u32(record->event.parameter);
    platform_uart_write_byte((uint8_t)',');
    write_u32((uint32_t) record->phase);
    platform_uart_write_string("\r\n");
}

static void dump_trace(void)
{
    event_trace_record_t record;
    const size_t count = event_trace_count();

    platform_uart_write_string("timestamp,source,destination,signal,parameter,phase\r\n");

    for (size_t i = 0U; i < count; i++)
    {
        if (event_trace_get(i, &record))
        {
            write_csv_record(&record);
        }
    }
}

static void execute_command(uint32_t timestamp_ms)
{
    uint32_t blink_period_ms;

    if (string_equal(g_command, "help"))
    {
        platform_uart_write_string("help | led on | led off | blink <ms> | ""blink stop | status | trace | reset | error\r\n");
    }
    else if (string_equal(g_command, "led on"))
    {
        (void) dispatcher_post(EVENT_SOURCE_UART, EVENT_DESTINATION_LED, SIGNAL_LED_ON, 0UL, timestamp_ms);
    }
    else if (string_equal(g_command, "led off"))
    {
        (void) dispatcher_post(EVENT_SOURCE_UART, EVENT_DESTINATION_LED, SIGNAL_LED_OFF, 0UL, timestamp_ms);
    }
    else if (string_equal(g_command, "blink stop"))
    {
        (void) dispatcher_post(EVENT_SOURCE_UART, EVENT_DESTINATION_LED, SIGNAL_LED_BLINK_STOP, 0UL, timestamp_ms);
    }
    else if (string_starts_with(g_command, "blink ") && parse_u32(&g_command[6], &blink_period_ms) && (blink_period_ms >= 50UL)
        && (blink_period_ms <= 5000UL))
    {
        (void) dispatcher_post(EVENT_SOURCE_UART, EVENT_DESTINATION_LED, SIGNAL_LED_BLINK_START, blink_period_ms,
        timestamp_ms);
    }
    else if (string_equal(g_command, "status"))
    {
        (void) dispatcher_post(EVENT_SOURCE_UART, EVENT_DESTINATION_APP, SIGNAL_UART_STATUS_REQUEST, 0UL, timestamp_ms);
    }
    else if (string_equal(g_command, "trace"))
    {
        dump_trace();
    }
    else if (string_equal(g_command, "reset"))
    {
        (void) dispatcher_post(EVENT_SOURCE_UART, EVENT_DESTINATION_APP, SIGNAL_RESET_REQUEST, 0UL, timestamp_ms);
    }
    else if (string_equal(g_command, "error"))
    {
        (void) dispatcher_post(EVENT_SOURCE_UART, EVENT_DESTINATION_APP, SIGNAL_ERROR_DETECTED, 0UL, timestamp_ms);
    }
    else if (g_length > 0U)
    {
        platform_uart_write_string("ERR: unknown command\r\n");
    }
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

    switch ((signal_t) event->signal)
    {
        case SIGNAL_UART_RX_BYTE :
        {
            byte = (uint8_t)(event->parameter & 0xFFUL);

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
                g_command[g_length] = (char) byte;
                g_length++;
            }
            else
            {
                g_length = 0U;
                g_command[0] = '\0';
                platform_uart_write_string("\r\nERR: command too long\r\n> ");
            }
            break;
        }

        case SIGNAL_UART_STATUS_BOOT : platform_uart_write_string("APP state: BOOT\r\n");
        break;

        case SIGNAL_UART_STATUS_IDLE : platform_uart_write_string("APP state: IDLE\r\n");
        break;

        case SIGNAL_UART_STATUS_ACTIVE : platform_uart_write_string("APP state: ACTIVE\r\n");
        break;

        case SIGNAL_UART_STATUS_ERROR : platform_uart_write_string("APP state: ERROR\r\n");
        break;

        default:
        break;
    }
}

size_t uart_service_command_length(void)
{
    return g_length;
}
