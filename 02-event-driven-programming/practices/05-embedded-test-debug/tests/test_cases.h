#ifndef TEST_CASES_H
#define TEST_CASES_H

#include <stdbool.h>

bool test_queue_fifo(void);
bool test_queue_overflow(void);
bool test_pool_allocate_release(void);
bool test_pool_reference_count(void);
bool test_timer_one_shot(void);
bool test_timer_periodic(void);
bool test_app_transitions(void);
bool test_app_error_recovery(void);
bool test_shell_commands(void);
bool test_trace_wraparound(void);
bool test_crash_record_validation(void);
bool test_fault_injection_flags(void);

#endif
