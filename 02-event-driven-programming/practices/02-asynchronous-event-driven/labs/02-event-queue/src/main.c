#include "event_queue.h"
#include <stdio.h>

int main(void)
{
    event_queue_t queue;
    event_t event;
    event_t output;

    event_queue_init(&queue);

    for (uint32_t i = 0UL; i < 5UL; i++)
    {
        event.sequence = i;
        event.signal = (uint16_t)(100U + i);
        (void) event_queue_post(&queue, &event);
    }

    while (event_queue_get(&queue, &output))
    {
        printf("sequence=%lu signal=%u\n", (unsigned long) output.sequence, (unsigned int) output.signal);
    }

    return 0;
}
