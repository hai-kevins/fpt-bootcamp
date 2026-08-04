#include "event_queue.h"

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
    queue->post_count = 0UL;
    queue->get_count = 0UL;
    queue->overflow_count = 0UL;
}

bool event_queue_post(event_queue_t *queue, const event_t *event)
{
    if ((queue == NULL) || (event == NULL))
    {
        return false;
    }

    if (queue->count >= EVENT_QUEUE_CAPACITY)
    {
        queue->overflow_count++;
        return false;
    }

    queue->buffer[queue->head] = *event;
    queue->head = (uint16_t)((queue->head + 1U) % EVENT_QUEUE_CAPACITY);
    queue->count++;
    queue->post_count++;

    if (queue->count > queue->high_water_mark)
    {
        queue->high_water_mark = queue->count;
    }

    return true;
}

bool event_queue_get(event_queue_t *queue, event_t *event)
{
    if ((queue == NULL) || (event == NULL))
    {
        return false;
    }

    if (queue->count == 0U)
    {
        return false;
    }

    *event = queue->buffer[queue->tail];
    queue->tail = (uint16_t)((queue->tail + 1U) % EVENT_QUEUE_CAPACITY);
    queue->count--;
    queue->get_count++;

    return true;
}

bool event_queue_is_empty(const event_queue_t *queue)
{
    return (queue == NULL) || (queue->count == 0U);
}

bool event_queue_is_full(const event_queue_t *queue)
{
    return (queue != NULL) && (queue->count >= EVENT_QUEUE_CAPACITY);
}

uint16_t event_queue_count(const event_queue_t *queue)
{
    return (queue == NULL) ? 0U : queue->count;
}
