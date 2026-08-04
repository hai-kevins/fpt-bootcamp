#include "model.h"

bool tick_reached(uint32_t n, uint32_t d)
{
    return (int32_t)(n - d) >= 0;
}

void periodic_init(periodic_t *p, uint32_t n, uint32_t q)
{
    p->next = n;
    p->period = q;
    p->misses = 0;
}

uint32_t delay_until(periodic_t *p, uint32_t n)
{
    p->next += p->period;
    if (tick_reached(n, p->next))
    {
        ++p->misses;
        return 0;
    }
    return p->next - n;
}

uint32_t relative_next(uint32_t n, uint32_t p)
{
    return n + p;
}
