#include "app_sm.h"
#include "crash_record.h"
#include "event_pool.h"
#include "event_queue.h"
#include "event_trace.h"
#include "fake_time.h"
#include "fault_injection.h"
#include "shell.h"
#include "software_timer.h"

#include <stdio.h>

static void dispatch_all(event_queue_t *queue, app_sm_t *app, event_trace_t *trace)
{
    event_t event;

    while (event_queue_get(queue, &event))
    {
        event_trace_record_t record =
        {
            .timestamp_ms = event.timestamp_ms,
            .signal = event.signal,
            .argument = (uint16_t) event.argument,
            .type = TRACE_TYPE_DISPATCH,
            .source = (uint8_t) event.source,
            .destination = 1U,
            .state = (uint8_t) app->state
        };

        event_trace_write(trace, &record);
        (void) app_sm_dispatch(app, &event);
    }
}

int main(void)
{
    event_queue_t queue;
    event_pool_t pool;
    fake_time_t time_source;
    software_timer_service_t timers;
    app_sm_t app;
    event_trace_t trace;
    fault_injection_t faults;
    crash_record_t crash;
    char output[160];

    (void) event_queue_init(&queue, 8U);
    (void) event_pool_init(&pool, 4U);
    fake_time_reset(&time_source);
    software_timer_init(&timers);
    app_sm_init(&app);
    event_trace_init(&trace);
    fault_injection_init(&faults);

    shell_context_t shell =
    {
        .app = &app,
        .queue = &queue,
        .pool = &pool,
        .trace = &trace,
        .faults = &faults
    };

    (void) shell_execute(&shell, "event start", output, sizeof(output));

    dispatch_all(&queue, &app, &trace);

    event_t timeout =
    {
        .signal = EVENT_SIGNAL_APP_TIMEOUT,
        .source = 2U,
        .argument = 0U,
        .timestamp_ms = 0U
    };

    (void) software_timer_start(&timers, 0U, fake_time_now(&time_source), 100U, false, &timeout);

    fake_time_advance(&time_source, 100U);
    software_timer_process(&timers, fake_time_now(&time_source), &queue);
    dispatch_all(&queue, &app, &trace);

    event_t *diagnostic = event_pool_allocate(&pool, EVENT_SIGNAL_DIAGNOSTIC);

    if (diagnostic != NULL)
    {
        diagnostic->argument = 0x1234U;
        (void) event_pool_release(&pool, diagnostic);
    }

    crash_record_prepare(&crash);
    crash.build_id = 0x05010000UL;
    crash.reset_reason = 1U;
    crash.fatal_code = 0xE001U;
    crash.uptime_ms = fake_time_now(&time_source);
    crash.last_signal = app.last_signal;
    crash.current_state = (uint8_t) app.state;
    crash.current_component = 1U;
    crash.queue_overflow_count = queue.overflow_count;
    crash.pool_failure_count = pool.allocation_failures;
    crash_record_finalize(&crash);

    (void) shell_execute(&shell, "stats", output, sizeof(output));

    (void) printf("Embedded Test/Debug demo\n");
    (void) printf("state=%s transitions=%lu\n", app_sm_state_name(app.state), (unsigned long) app.transition_count);
    (void) printf("timer_expiry=%lu trace=%zu\n", (unsigned long) timers.expiry_count, trace.count);
    (void) printf("%s\n", output);
    (void) printf("crash_record=%s\n", crash_record_is_valid(&crash) ? "VALID" : "INVALID");

    return 0;
}
