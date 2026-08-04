#include "model.h"
#include <stdio.h>

int main(void)
{
    periodic_t p;
    periodic_init(&p, 0, 10);
    uint32_t now = 0, relative = 0;
    for (int i = 0; i < 5; ++i)
    {
        now += 2;
        uint32_t wait = delay_until(&p, now);
        relative = relative_next(now, 10);
        printf("cycle=%d absolute=%u relative=%u wait=%u\n", i, p.next, relative, wait);
        now = p.next;
    }
    return 0;
}
