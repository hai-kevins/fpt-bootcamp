#include "task_shell.h"

#include "app.h"
#include "app_signals.h"
#include "app_tasks.h"
#include "event_record.h"
#include "fatal.h"
#include "message.h"
#include "task.h"
#include "timer.h"
#include "uart.h"

#include <stdbool.h>
#include <stdint.h>
#include "runtime.h"

#define SHELL_LINE_CAPACITY (48U)

static char g_line[SHELL_LINE_CAPACITY];
static uint8_t g_length;

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
        digits[count++] = (char)('0' + (value % 10U));
        value /= 10U;
    }
    while (count > 0U)
    {
        uart_write_byte((uint8_t)digits[--count]);
    }
}

static void post_pure(uint8_t destination, uint16_t signal)
{
    ak_message_t *message = ak_message_create_pure(
        APP_TASK_SHELL, destination, signal
    );
    if ((message != 0) && !ak_task_post(message))
    {
        ak_message_release(message);
    }
}

static void execute_line(void)
{
    if (strcmp(g_line, "help") == 0)
    {
        uart_write_string(
            "help task pool timer state led on|off|toggle "
            "event dump|clear fatal show|test|clear error reset\r\n"
        );
    }
    else if (strcmp(g_line, "task") == 0)
    {
        for (uint8_t id = 1U; id <= 5U; id++)
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
    }
    else if (strcmp(g_line, "pool") == 0)
    {
        const ak_message_pool_stats_t stats = ak_message_pool_stats();
        uart_write_string("pool used=");
        write_u32(stats.used);
        uart_write_string(" max=");
        write_u32(stats.used_max);
        uart_write_string(" fail=");
        write_u32(stats.allocation_failures);
        uart_write_string("\r\n");
    }
    else if (strcmp(g_line, "timer") == 0)
    {
        const ak_timer_stats_t stats = ak_timer_stats();
        uart_write_string("timer active=");
        write_u32(stats.active_count);
        uart_write_string(" max=");
        write_u32(stats.active_max);
        uart_write_string(" expire=");
        write_u32(stats.expirations);
        uart_write_string("\r\n");
    }
    else if (strcmp(g_line, "state") == 0)
    {
        uart_write_string("app state=");
        write_u32(app_state_get());
        uart_write_string("\r\n");
    }
    else if (strcmp(g_line, "led on") == 0)
    {
        post_pure(APP_TASK_LED, APP_SIGNAL_LED_ON);
    }
    else if (strcmp(g_line, "led off") == 0)
    {
        post_pure(APP_TASK_LED, APP_SIGNAL_LED_OFF);
    }
    else if (strcmp(g_line, "led toggle") == 0)
    {
        post_pure(APP_TASK_LED, APP_SIGNAL_LED_TOGGLE);
    }
    else if (strcmp(g_line, "event clear") == 0)
    {
        ak_event_record_clear();
        uart_write_string("event record cleared\r\n");
    }
    else if (strcmp(g_line, "event dump") == 0)
    {
        uart_write_string("event count=");
        write_u32((uint32_t)ak_event_record_count());
        uart_write_string(" overwritten=");
        write_u32(ak_event_record_overwritten());
        uart_write_string("\r\n");
    }
    else if (strcmp(g_line, "fatal show") == 0)
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
    }
    else if (strcmp(g_line, "fatal clear") == 0)
    {
        ak_fatal_clear();
        uart_write_string("fatal cleared\r\n");
    }
    else if (strcmp(g_line, "fatal test") == 0)
    {
        ak_fatal_raise(1001U, APP_TASK_SHELL, APP_SIGNAL_SHELL_COMMAND);
    }
    else if (strcmp(g_line, "error") == 0)
    {
        post_pure(APP_TASK_APPLICATION, APP_SIGNAL_APP_ERROR);
    }
    else if (strcmp(g_line, "reset") == 0)
    {
        post_pure(APP_TASK_APPLICATION, APP_SIGNAL_APP_RESET);
    }
    else if (g_length > 0U)
    {
        uart_write_string("unknown command\r\n");
    }
}

void task_shell_init(void)
{
    g_length = 0U;
    g_line[0] = '\0';
}

void task_shell_handler(const ak_message_t *message)
{
    if ((message->signal != APP_SIGNAL_UART_RX_BYTE) ||
        (message->length != 1U))
    {
        return;
    }

    const uint8_t byte = message->payload[0];
    if ((byte == '\r') || (byte == '\n'))
    {
        if (g_length > 0U)
        {
            g_line[g_length] = '\0';
            execute_line();
            g_length = 0U;
            g_line[0] = '\0';
        }
    }
    else if ((byte == 0x08U) || (byte == 0x7FU))
    {
        if (g_length > 0U)
        {
            g_length--;
        }
    }
    else if (g_length < (SHELL_LINE_CAPACITY - 1U))
    {
        g_line[g_length++] = (char)byte;
    }
}
