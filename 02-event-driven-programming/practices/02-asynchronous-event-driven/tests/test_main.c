#include "test.h"

#include <stdio.h>

bool test_event_queue_fifo(void);
bool test_event_queue_wrap_and_overflow(void);
bool test_timer_one_shot(void);
bool test_timer_periodic(void);
bool test_button_debounce_press(void);
bool test_led_state_machine(void);
bool test_app_state_machine(void);
bool test_cmd_line_parser(void);

int main(void)
{
    test_run("event_queue_fifo", test_event_queue_fifo);
    test_run("event_queue_wrap_and_overflow", test_event_queue_wrap_and_overflow);
    test_run("timer_one_shot", test_timer_one_shot);
    test_run("timer_periodic", test_timer_periodic);
    test_run("button_debounce_press", test_button_debounce_press);
    test_run("led_state_machine", test_led_state_machine);
    test_run("app_state_machine", test_app_state_machine);
    test_run("cmd_line_parser", test_cmd_line_parser);

    printf("\nSummary: passed=%lu failed=%lu\n", (unsigned long) g_test_passed, (unsigned long) g_test_failed);

    return (g_test_failed == 0UL) ? 0 : 1;
}
