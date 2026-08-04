#include "ring_queue.h"
#include <stdio.h>

int main(void)
{
    ring_queue_t q;
    int storage[4];
    ring_queue_init(&q, storage, 4, sizeof(int));
    for (int i = 10; i < 14; i++)
    {
        ring_queue_send(&q, &i);
    }
    while (q.count)
    {
        int v;
        ring_queue_receive(&q, &v);
        printf("%d\n", v);
    }
    return 0;
}
