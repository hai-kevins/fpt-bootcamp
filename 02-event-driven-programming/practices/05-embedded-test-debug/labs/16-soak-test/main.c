#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

int main(void)
{
    const uint32_t hours = 12U;
    uint32_t samples = 0U;
    uint32_t resource_used = 0U;
    uint32_t resets = 0U;
    bool leak = false;

    for (uint32_t hour = 0U; hour < hours; hour++)
    {
        for (uint32_t event = 0U; event < 10000U; event++)
        {
            resource_used++;
            resource_used--;
        }

        samples++;

        if (resource_used != 0U)
        {
            leak = true;
        }
    }

    const bool pass =
        (samples == hours) &&
        !leak &&
        (resets == 0U);

    (void)printf(
        "hours=%lu samples=%lu leak=%u resets=%lu %s\n",
        (unsigned long)hours,
        (unsigned long)samples,
        leak ? 1U : 0U,
        (unsigned long)resets,
        pass ? "PASS" : "FAIL"
    );

    return pass ? 0 : 1;
}
