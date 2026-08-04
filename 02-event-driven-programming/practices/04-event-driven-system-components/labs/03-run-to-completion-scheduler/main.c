#include "active_object.h"
#include "event_pool.h"
#include "scheduler.h"
#include <stdio.h>
static unsigned order[2], n;
static void h(ed_active_object_t *o, const ed_event_t *e)
{
    (void) e;
    order[n++] = o->id;
}

int main(void)
{
    ed_event_pool_t p;
    ed_scheduler_t s;
    ed_active_object_t l, hi;
    ed_event_t a, b;
    ed_event_pool_init(&p);
    ed_scheduler_init(&s, &p);
    ed_active_object_init(&l, 1U, 1U, "low", 2U, h, NULL);
    ed_active_object_init(&hi, 2U, 3U, "high", 2U, h, NULL);
    ed_scheduler_register(&s, &l);
    ed_scheduler_register(&s, &hi);
    ed_event_init_static(&a, 1U, 0U, 1U);
    ed_event_init_static(&b, 1U, 0U, 2U);
    ed_active_object_post(&l, &a);
    ed_active_object_post(&hi, &b);
    ed_scheduler_run_until_idle(&s, 4U);
    (void) printf("order=%u,%u\n", order[0], order[1]);
    return (n == 2U && order[0] == 2U && order[1] == 1U) ? 0 : 1;
}
