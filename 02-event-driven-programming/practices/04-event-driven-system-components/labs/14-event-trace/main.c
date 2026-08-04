#include "event_trace.h"
#include <stdio.h>
int main(void)
{
    ed_trace_t t;
    ed_trace_record_t r =
    {
        0
    },
    o;
    ed_trace_init(&t);
    for (size_t i = 0; i < ED_TRACE_CAPACITY + 5U; i++)
    {
        r.timestamp = (uint32_t) i;
        r.signal = (ed_signal_t) i;
        ed_trace_write(&t, &r);
    }
    ed_trace_get_oldest(&t, 0U, &o);
    (void) printf("count=%zu overwritten=%lu oldest=%lu\n", t.count, (unsigned long) t.overwritten_count, (unsigned long) o.timestamp);
    return (t.count == ED_TRACE_CAPACITY && o.timestamp == 5U) ? 0 : 1;
}
