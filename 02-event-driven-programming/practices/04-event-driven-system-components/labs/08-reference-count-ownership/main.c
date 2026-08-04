#include "event_pool.h"
#include <stdio.h>

int main(void)
{
    ed_event_pool_t p;
    ed_event_pool_init(&p);
    ed_event_t *e = ed_event_pool_allocate(&p, 1U, 0U, 0U);
    if (e == NULL)
    {
        return 1;
    }
    ed_event_pool_retain(&p, e);
    unsigned r1 = e->ref_count;
    ed_event_pool_release(&p, e);
    unsigned r2 = e->ref_count;
    ed_event_pool_release(&p, e);
    (void) printf("refs=%u->%u used=%zu\n", r1, r2, p.used_count);
    return (r1 == 2U && r2 == 1U && p.used_count == 0U) ? 0 : 1;
}
