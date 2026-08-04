#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

int main(void)
{
    const uint32_t post[] =
    {
        10U,
        20U,
        30U,
        40U
    };
    const uint32_t dispatch[] =
    {
        15U,
        30U,
        50U,
        50U
    };

    uint32_t minimum = UINT32_MAX;
    uint32_t maximum = 0U;
    uint32_t total = 0U;

    for (size_t i = 0U; i < 4U; i++)
    {
        const uint32_t latency = dispatch[i] - post[i];

        if (latency < minimum)
        {
            minimum = latency;
        }

        if (latency > maximum)
        {
            maximum = latency;
        }

        total += latency;
    }

    const uint32_t average = total / 4U;
    const bool pass = (minimum == 5U) && (maximum == 20U) && (average == 11U);

    (void) printf("min=%lu max=%lu avg=%lu %s\n", (unsigned long) minimum, (unsigned long) maximum, (unsigned long) average,
        pass ? "PASS" : "FAIL");

    return pass ? 0 : 1;
}
