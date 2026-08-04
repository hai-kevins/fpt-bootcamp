#include "event_queue.h"
#include <stdbool.h>
#include <stdio.h>

static bool test_fifo(void)
{
    event_queue_t queue;
    event_t event;
    event_t output;

    event_queue_init(&queue);

    for (uint32_t i = 0UL; i < 4UL; i++)
    {
        event.sequence = i;
        event.signal = 1U;
        if (!event_queue_post(&queue, &event))
        {
            return false;
        }
    }

    for (uint32_t i = 0UL; i < 4UL; i++)
    {
        if (!event_queue_get(&queue, &output) || (output.sequence != i))
        {
            return false;
        }
    }

    return true;
}

static bool test_wraparound_and_full(void)
{
    event_queue_t queue;
    event_t event =
    {
        0
    };
    event_t output;

    event_queue_init(&queue);

    for (uint32_t i = 0UL; i < LAB02_QUEUE_CAPACITY; i++)
    {
        event.sequence = i;
        if (!event_queue_post(&queue, &event))
        {
            return false;
        }
    }

    if (event_queue_post(&queue, &event) || (queue.overflow_count != 1UL))
    {
        return false;
    }

    for (uint32_t i = 0UL; i < 3UL; i++)
    {
        if (!event_queue_get(&queue, &output))
        {
            return false;
        }
    }

    for (uint32_t i = 0UL; i < 3UL; i++)
    {
        event.sequence = 100UL + i;
        if (!event_queue_post(&queue, &event))
        {
            return false;
        }
    }

    return queue.high_water_mark == LAB02_QUEUE_CAPACITY;
}

int main(void)
{
    const bool fifo = test_fifo();
    const bool wrap = test_wraparound_and_full();

    printf("[%s] fifo\n", fifo ? "PASS" : "FAIL");
    printf("[%s] wraparound-full\n", wrap ? "PASS" : "FAIL");

    return (fifo && wrap) ? 0 : 1;
}
