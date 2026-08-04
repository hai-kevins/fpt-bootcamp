#include "shell.h"

#include "statistics.h"

#include <stdio.h>
#include <string.h>

static int write_help(char *output, size_t output_size)
{
    return snprintf(output, output_size, "help | state | event start|stop|fail|reset | ""stats | trace | fault pool on|off");
}

static int post_signal(shell_context_t *context, uint16_t signal, char *output, size_t output_size)
{
    event_t event =
    {
        .signal = signal,
        .source = 0U,
        .argument = 0U,
        .timestamp_ms = 0U
    };

    const bool posted = event_queue_post(context->queue, &event);

    return snprintf(output, output_size, "%s", posted ? "OK" : "QUEUE_FULL");
}

int shell_execute(shell_context_t *context, const char *line, char *output, size_t output_size)
{
    if ((context == NULL) || (line == NULL) || (output == NULL) || (output_size == 0U))
    {
        return -1;
    }

    if (strcmp(line, "help") == 0)
    {
        return write_help(output, output_size);
    }

    if (strcmp(line, "state") == 0)
    {
        return snprintf(output, output_size, "state=%s transitions=%lu unhandled=%lu", app_sm_state_name(context->app->state),
        (unsigned long) context->app->transition_count, (unsigned long) context->app->unhandled_count);
    }

    if (strcmp(line, "event start") == 0)
    {
        return post_signal(context, EVENT_SIGNAL_APP_START, output, output_size);
    }

    if (strcmp(line, "event stop") == 0)
    {
        return post_signal(context, EVENT_SIGNAL_APP_STOP, output, output_size);
    }

    if (strcmp(line, "event fail") == 0)
    {
        return post_signal(context, EVENT_SIGNAL_APP_FAIL, output, output_size);
    }

    if (strcmp(line, "event reset") == 0)
    {
        return post_signal(context, EVENT_SIGNAL_APP_RESET, output, output_size);
    }

    if (strcmp(line, "stats") == 0)
    {
        statistics_snapshot_t snapshot =
        {
            0
        };

        statistics_capture(&snapshot, context->queue, context->pool);

        return snprintf(output, output_size, "queue=%zu hwm=%zu overflow=%lu ""pool=%zu pool_hwm=%zu pool_fail=%lu",
        snapshot.queue_count, snapshot.queue_high_water, (unsigned long) snapshot.queue_overflow, snapshot.pool_used,
        snapshot.pool_high_water, (unsigned long) snapshot.pool_failures);
    }

    if (strcmp(line, "trace") == 0)
    {
        return snprintf(output, output_size, "trace=%zu overwritten=%lu", context->trace->count, (unsigned long) context->trace->overwritten_count);
    }

    if (strcmp(line, "fault pool on") == 0)
    {
        fault_injection_enable(context->faults, FAULT_INJECTION_POOL_FAILURE);
        return snprintf(output, output_size, "OK");
    }

    if (strcmp(line, "fault pool off") == 0)
    {
        fault_injection_disable(context->faults, FAULT_INJECTION_POOL_FAILURE);
        return snprintf(output, output_size, "OK");
    }

    return snprintf(output, output_size, "UNKNOWN_COMMAND");
}
