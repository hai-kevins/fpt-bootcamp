#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

int main(void)
{
    const uint32_t samples_us[] =
    {
        700U,
        850U,
        1420U,
        930U
    };

    uint32_t maximum = 0U;

    for (size_t i = 0U;
         i < sizeof(samples_us) / sizeof(samples_us[0]);
         i++)
    {
        if (samples_us[i] > maximum)
        {
            maximum = samples_us[i];
        }
    }

    const uint32_t threshold = 1000U;
    const bool exceeded = maximum > threshold;

    (void)printf(
        "DISPLAY handler max: %lu us "
        "threshold=%lu exceeded=%u\n",
        (unsigned long)maximum,
        (unsigned long)threshold,
        exceeded ? 1U : 0U
    );

    return (maximum == 1420U && exceeded) ? 0 : 1;
}
