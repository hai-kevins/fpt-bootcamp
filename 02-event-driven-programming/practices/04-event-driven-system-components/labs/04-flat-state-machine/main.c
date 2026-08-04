#include "fsm.h"
#include <stdio.h>
static unsigned actions;
static void a(void *c, const ed_event_t *e)
{
    (void) e;
    (*(unsigned *) c)++;
}

int main(void)
{
    const ed_fsm_transition_t t[] =
    {
        {
            0U, 1U, 1U, a
        },
        {
            1U,
            2U,
            0U,
            a
        }
    };
    ed_fsm_t f;
    ed_event_t e;
    ed_fsm_init(&f, 0U, t, 2U, &actions);
    ed_event_init_static(&e, 1U, 0U, 0U);
    ed_fsm_dispatch(&f, &e);
    e.signal = 2U;
    ed_fsm_dispatch(&f, &e);
    (void) printf("state=%u actions=%u\n", f.state, actions);
    return (f.state == 0U && actions == 2U) ? 0 : 1;
}
