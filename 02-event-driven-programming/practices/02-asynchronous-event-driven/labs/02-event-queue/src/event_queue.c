#include "event_queue.h"
#include <stddef.h>

void event_queue_init(event_queue_t *queue)
{
    if (queue == NULL)
    {
        return;
    }
    queue->head = 0U;
    queue->tail = 0U;
    queue->count = 0U;
    queue->high_water_mark = 0U;
    queue->overflow_count = 0UL;
}

bool event_queue_post(event_queue_t *queue, const event_t *event)
{
    if ((queue == NULL) || (event == NULL))
    {
        return false;
    }

    if (queue->count >= LAB02_QUEUE_CAPACITY)
    {
        queue->overflow_count++;
        return false;
    }

    queue->buffer[queue->head] = *event;
    queue->head =
        (uint16_t)((queue->head + 1U) %
                   LAB02_QUEUE_CAPACITY);
    queue->count++;

    if (queue->count > queue->high_water_mark)
    {
        queue->high_water_mark = queue->count;
    }

    return true;
}

bool event_queue_get(event_queue_t *queue, event_t *event)
{
    if ((queue == NULL) || (event == NULL) ||
        (queue->count == 0U))
    {
        return false;
    }

    *event = queue->buffer[queue->tail];
    queue->tail =
        (uint16_t)((queue->tail + 1U) %
                   LAB02_QUEUE_CAPACITY);
    queue->count--;

    return true;
}
