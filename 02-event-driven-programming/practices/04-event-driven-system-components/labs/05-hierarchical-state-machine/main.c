#include "hsm.h"
#include <stdio.h>

static unsigned hits;

static bool h(void *c, uint8_t s, const ed_event_t *e, uint8_t *n)
{
    (void) c;
    if (s == 0U && e->signal == 9U)
    {
        hits++;
        *n = 1U;
        return true;
    }
    return false;
}

int main(void)
{
    const uint8_t p[] =
    {
        ED_HSM_NO_STATE,
        0U,
        0U
    };
    ed_hsm_t m;
    ed_event_t e;
    ed_hsm_init(&m, 2U, p, 3U, h, NULL);
    ed_event_init_static(&e, 9U, 0U, 0U);
    int ok = ed_hsm_dispatch(&m, &e);
    (void) printf("state=%u bubbled=%lu\n", m.current_state, (unsigned long) m.bubbled_count);
    return (ok && hits == 1U && m.current_state == 1U) ? 0 : 1;
}
