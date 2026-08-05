#include <stdbool.h>
#include <stdio.h>

typedef bool (*test_fn_t)(void);

typedef struct
{
    const char *name;
    test_fn_t fn;
} test_entry_t;

bool test_message_pool(void);
bool test_task_scheduler(void);
bool test_timer(void);
bool test_fsm_and_tsm(void);
bool test_event_record(void);
bool test_fatal_record(void);
bool test_application_flow(void);
bool test_cmd_line_parser(void);

int main(void)
{
    const test_entry_t tests[] =
    {
        {
            "message_pool", test_message_pool
        },
        {
            "task_scheduler",
            test_task_scheduler
        },
        {
            "timer",
            test_timer
        },
        {
            "fsm_and_tsm",
            test_fsm_and_tsm
        },
        {
            "event_record",
            test_event_record
        },
        {
            "fatal_record",
            test_fatal_record
        },
        {
            "application_flow",
            test_application_flow
        },
        {
            "cmd_line_parser",
            test_cmd_line_parser
        }
    };
    unsigned passed = 0U;
    for (unsigned i = 0U; i < sizeof(tests) / sizeof(tests[0]); i++)
    {
        if (tests[i].fn())
        {
            printf("[PASS] %s\n", tests[i].name);
            passed++;
        }
        else
        {
            printf("[FAIL] %s\n", tests[i].name);
        }
    }
    printf("Summary: %u/%zu passed\n", passed, sizeof(tests) / sizeof(tests[0]));
    return (passed == sizeof(tests) / sizeof(tests[0])) ? 0 : 1;
}
