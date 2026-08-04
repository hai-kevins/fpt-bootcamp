#include "shell.h"
#include "test_assert.h"
#include "test_cases.h"

#include <string.h>

bool test_shell_commands(void)
{
    app_sm_t app;
    event_queue_t queue;
    event_pool_t pool;
    event_trace_t trace;
    fault_injection_t faults;
    char output[160];

    app_sm_init(&app);
    TEST_ASSERT_TRUE(event_queue_init(&queue, 4U));
    TEST_ASSERT_TRUE(event_pool_init(&pool, 2U));
    event_trace_init(&trace);
    fault_injection_init(&faults);

    shell_context_t context =
    {
        .app = &app,
        .queue = &queue,
        .pool = &pool,
        .trace = &trace,
        .faults = &faults
    };

    TEST_ASSERT_TRUE(shell_execute(&context, "event start", output, sizeof(output)) >= 0);
    TEST_ASSERT_TRUE(strcmp(output, "OK") == 0);
    TEST_ASSERT_EQ(1U, queue.count);

    TEST_ASSERT_TRUE(shell_execute(&context, "fault pool on", output, sizeof(output)) >= 0);
    TEST_ASSERT_TRUE(fault_injection_is_enabled(&faults, FAULT_INJECTION_POOL_FAILURE));

    TEST_ASSERT_TRUE(shell_execute(&context, "unknown", output, sizeof(output)) >= 0);
    TEST_ASSERT_TRUE(strcmp(output, "UNKNOWN_COMMAND") == 0);
    return true;
}
