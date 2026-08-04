#include "pubsub.h"
#include "scheduler.h"
#include <stdio.h>
static unsigned hits;
static void h(ed_active_object_t *o, const ed_event_t *e)
{
    (void) o;
    (void) e;
    hits++;
}

int main(void)
{
    ed_event_pool_t p;
    ed_pubsub_t ps;
    ed_scheduler_t s;
    ed_active_object_t a, b;
    ed_event_t e;
    ed_event_pool_init(&p);
    ed_pubsub_init(&ps, &p);
    ed_scheduler_init(&s, &p);
    ed_active_object_init(&a, 1U, 1U, "a", 4U, h, NULL);
    ed_active_object_init(&b, 2U, 1U, "b", 4U, h, NULL);
    ed_scheduler_register(&s, &a);
    ed_scheduler_register(&s, &b);
    ed_pubsub_subscribe(&ps, 5U, &a);
    ed_pubsub_subscribe(&ps, 5U, &b);
    ed_event_init_static(&e, 5U, 0U, 0U);
    size_t d = ed_pubsub_publish(&ps, &e);
    ed_scheduler_run_until_idle(&s, 4U);
    (void) printf("delivered=%zu hits=%u\n", d, hits);
    return (d == 2U && hits == 2U && p.used_count == 0U) ? 0 : 1;
}
