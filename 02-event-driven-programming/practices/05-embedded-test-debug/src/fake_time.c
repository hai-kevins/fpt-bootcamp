#include "fake_time.h"

#include <stddef.h>

void fake_time_reset(fake_time_t *time_source)
{
    if (time_source != NULL)
    {
        time_source->now_ms = 0U;
    }
}

uint32_t fake_time_now(const fake_time_t *time_source)
{
    return (time_source == NULL) ? 0U : time_source->now_ms;
}

void fake_time_advance(fake_time_t *time_source, uint32_t delta_ms)
{
    if (time_source != NULL)
    {
        time_source->now_ms += delta_ms;
    }
}
