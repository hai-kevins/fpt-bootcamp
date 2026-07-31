#include "test_cases.h"
#include "test_runner.h"

#include <stddef.h>

int main(void)
{
    const test_case_t tests[] =
    {
        {"event_queue_fifo", test_queue_fifo},
        {"event_queue_overflow", test_queue_overflow},
        {"event_pool_allocate_release", test_pool_allocate_release},
        {"event_pool_reference_count", test_pool_reference_count},
        {"software_timer_one_shot", test_timer_one_shot},
        {"software_timer_periodic", test_timer_periodic},
        {"app_state_transitions", test_app_transitions},
        {"app_error_recovery", test_app_error_recovery},
        {"uart_shell_commands", test_shell_commands},
        {"event_trace_wraparound", test_trace_wraparound},
        {"crash_record_validation", test_crash_record_validation},
        {"fault_injection_flags", test_fault_injection_flags}
    };

    return test_run_all(
        "05-embedded-test-debug tests",
        tests,
        sizeof(tests) / sizeof(tests[0])
    );
}
