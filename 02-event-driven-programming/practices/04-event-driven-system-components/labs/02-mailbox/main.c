#include "mailbox.h"
#include <stdio.h>

int main(void)
{
    ed_mailbox_t q;
    ed_event_t a, b, c;
    ed_mailbox_init(&q, 2U, ED_MAILBOX_DROP_NEWEST);
    ed_event_init_static(&a, 1U, 0U, 0U);
    ed_event_init_static(&b, 2U, 0U, 0U);
    ed_event_init_static(&c, 3U, 0U, 0U);
    int ok = ed_mailbox_post(&q, &a) && ed_mailbox_post(&q, &b) && !ed_mailbox_post(&q, &c) && ed_mailbox_get(&q)->signal == 1U && ed_mailbox_get(&q)->signal == 2U;
    (void) printf("hwm=%zu overflow=%lu\n", q.high_water_mark, (unsigned long) q.overflow_count);
    return ok ? 0 : 1;
}
