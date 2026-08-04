#include "datalink.h"
#include <stdio.h>

static unsigned sends;

static bool tx(void *c, const uint8_t *d, size_t n)
{
    (void) c;
    if (d == NULL || n == 0U)
    {
        return false;
    }
    sends++;
    return true;
}

int main(void)
{
    ed_datalink_t l;
    ed_frame_t f =
    {
        0
    };
    f.sequence = 12U;
    ed_datalink_init(&l, tx, NULL, 5U, 2U);
    ed_datalink_send(&l, &f, true);
    for (unsigned i = 0; i < 3U; i++)
    {
        ed_datalink_tick(&l, 5U);
    }
    (void) printf("state=%u sends=%u retries=%lu\n", l.state, sends, (unsigned long) l.retry_total);
    return (l.state == ED_LINK_DOWN && sends == 3U) ? 0 : 1;
}
