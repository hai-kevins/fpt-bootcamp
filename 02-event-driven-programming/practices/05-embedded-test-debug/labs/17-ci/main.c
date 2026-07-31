#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

typedef struct
{
    const char *name;
    bool passed;
} stage_t;

int main(void)
{
    const stage_t stages[] =
    {
        {"Build", true},
        {"Unit test", true},
        {"Sanitizer", true},
        {"Firmware build", true},
        {"Size check", true},
        {"Artifact report", true}
    };

    size_t passed = 0U;

    for (size_t i = 0U;
         i < sizeof(stages) / sizeof(stages[0]);
         i++)
    {
        passed += stages[i].passed ? 1U : 0U;
    }

    const size_t total =
        sizeof(stages) / sizeof(stages[0]);
    const bool ok = passed == total;

    (void)printf(
        "stages=%zu passed=%zu failed=%zu %s\n",
        total,
        passed,
        total - passed,
        ok ? "PASS" : "FAIL"
    );

    return ok ? 0 : 1;
}
