#include "active_object.h"
#include "scheduler.h"
#include <stdio.h>

static unsigned handled;

static void h(ed_active_object_t *o, const ed_event_t *e)
{
    (void) o;
    (void) e;
    handled++;
}

int main(void)
{
    ed_event_pool_t p;
    ed_scheduler_t s;
    ed_active_object_t a;
    ed_event_pool_init(&p);
    ed_scheduler_init(&s, &p);
    ed_active_object_init(&a, 1U, 1U, "stress", 32U, h, NULL);
    ed_scheduler_register(&s, &a);
    unsigned posted = 0;
    for (unsigned i = 0; i < 1000U; i++)
    {
        ed_event_t *e = ed_event_pool_allocate(&p, 1U, 0U, 1U);
        if (e != NULL)
        {
            if (ed_active_object_post(&a, e))
            {
                posted++;
            }
            else
            {
                ed_event_pool_release(&p, e);
            }
        }
        if ((i % 16U) == 15U)
        {
            ed_scheduler_run_until_idle(&s, 32U);
        }
    }
    ed_scheduler_run_until_idle(&s, 64U);
    (void) printf("posted=%u handled=%u hwm=%zu failures=%lu\n", posted, handled, a.mailbox.high_water_mark, (unsigned long) p.allocation_failure_count);
    return (posted == handled && p.used_count == 0U) ? 0 : 1;
}
