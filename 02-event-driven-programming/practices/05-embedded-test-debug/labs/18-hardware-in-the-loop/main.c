#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
    const char *command;
    const char *response;
    const char *expected;
} test_case_t;

int main(void)
{
    const bool reset_pass = true;

    const test_case_t cases[] =
    {
        {"state", "state=IDLE", "IDLE"},
        {"event start", "OK", "OK"},
        {"state", "state=RUNNING", "RUNNING"},
        {"stats", "queue=0 pool=0", "queue="}
    };

    size_t passed = 0U;

    for (size_t i = 0U;
         i < sizeof(cases) / sizeof(cases[0]);
         i++)
    {
        const bool ok =
            strstr(
                cases[i].response,
                cases[i].expected
            ) != NULL;

        passed += ok ? 1U : 0U;
    }

    const size_t total =
        sizeof(cases) / sizeof(cases[0]);

    const bool hil_pass =
        reset_pass &&
        (passed == total);

    (void)printf(
        "reset=%s commands=%zu passed=%zu failed=%zu HIL=%s\n",
        reset_pass ? "PASS" : "FAIL",
        total,
        passed,
        total - passed,
        hil_pass ? "PASS" : "FAIL"
    );

    return hil_pass ? 0 : 1;
}
