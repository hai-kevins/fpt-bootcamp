#include "ring_queue.h"
#include <assert.h>
#include <stdio.h>

int main(void)
{
    ring_queue_t q;
    int s[3], x;
    assert(ring_queue_init(&q, s, 3, sizeof(int)));
    for (int i = 1; i <= 3; i++)
    {
        assert(ring_queue_send(&q, &i));
    }
    assert(!ring_queue_send(&q, &x));
    for (int i = 1; i <= 3; i++)
    {
        assert(ring_queue_receive(&q, &x));
        assert(x == i);
    }
    assert(!ring_queue_receive(&q, &x));
    for (int i = 4; i <= 8; i++)
    {
        assert(ring_queue_send(&q, &i));
        assert(ring_queue_receive(&q, &x));
        assert(x == i);
    }
    assert(ring_queue_validate(&q));
    puts("[PASS] static ring queue");
    return 0;
}
