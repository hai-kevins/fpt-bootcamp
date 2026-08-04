#include "event_queue.h"

#include <string.h>

bool event_queue_init(event_queue_t *queue, size_t capacity)
{
    if ((queue == NULL) || (capacity == 0U) || (capacity > EVENT_QUEUE_MAX_CAPACITY))
    {
        return false;
    }

    (void) memset(queue, 0, sizeof (*queue));
    queue->capacity = capacity;
    return true;
}

bool event_queue_post(event_queue_t *queue, const event_t *event)
{
    if ((queue == NULL) || (event == NULL))
    {
        return false;
    }

    if (queue->count >= queue->capacity)
    {
        queue->overflow_count++;
        return false;
    }

    queue->items[queue->tail] = *event;
    queue->tail = (queue->tail + 1U) % queue->capacity;
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
    if ((queue == NULL) || (event == NULL) || (queue->count == 0U))
    {
        return false;
    }

    *event = queue->items[queue->head];
    queue->head = (queue->head + 1U) % queue->capacity;
    queue->count--;
    queue->get_count++;
    return true;
}

bool event_queue_is_empty(const event_queue_t *queue)
{
    return (queue == NULL) || (queue->count == 0U);
}
