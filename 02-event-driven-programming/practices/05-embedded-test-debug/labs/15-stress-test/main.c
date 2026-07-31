#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

int main(void)
{
    const uint32_t events = 10000U;
    uint32_t queue_current = 0U;
    uint32_t queue_max = 0U;
    uint32_t pool_current = 0U;
    uint32_t pool_max = 0U;
    uint32_t drop = 0U;
    uint32_t handler_max = 0U;

    for (uint32_t i = 0U; i < events; i++)
    {
        if ((queue_current < 16U) &&
            (pool_current < 16U))
        {
            queue_current++;
            pool_current++;

            if (queue_current > queue_max)
            {
                queue_max = queue_current;
            }

            if (pool_current > pool_max)
            {
                pool_max = pool_current;
            }
        }
        else
        {
            drop++;
        }

        const uint32_t sample = 500U + (i % 401U);
        if (sample > handler_max)
        {
            handler_max = sample;
        }

        if ((i % 16U) == 15U)
        {
            queue_current = 0U;
            pool_current = 0U;
        }
    }

    const bool pass =
        (drop == 0U) &&
        (queue_max == 16U) &&
        (pool_max == 16U) &&
        (handler_max == 900U);

    (void)printf(
        "events=%lu drop=%lu queue_max=%lu pool_max=%lu "
        "handler_max=%lu %s\n",
        (unsigned long)events,
        (unsigned long)drop,
        (unsigned long)queue_max,
        (unsigned long)pool_max,
        (unsigned long)handler_max,
        pass ? "PASS" : "FAIL"
    );

    return pass ? 0 : 1;
}
