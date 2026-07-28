#include "queue.h"
#include "critical_section.h"
#include "scheduler.h"
#include "wait_list.h"
#include <stddef.h>

void *memcpy(void *destination, const void *source, size_t count);

static void copy_into_tail(hr_queue_t *queue, const void *item)
{
    uint8_t *destination = queue->storage + (queue->tail * queue->item_size);
    (void)memcpy(destination, item, queue->item_size);
    ++queue->tail;
    if (queue->tail == queue->capacity) { queue->tail = 0U; }
    ++queue->count;
    ++queue->send_count;
}

static void copy_from_head(hr_queue_t *queue, void *item)
{
    const uint8_t *source = queue->storage + (queue->head * queue->item_size);
    (void)memcpy(item, source, queue->item_size);
    ++queue->head;
    if (queue->head == queue->capacity) { queue->head = 0U; }
    --queue->count;
    ++queue->receive_count;
}

static bool higher_than_current(const hr_task_t *task)
{
    const hr_task_t *current = hr_scheduler_current();
    return (task != (const hr_task_t *)0) &&
           ((current == (const hr_task_t *)0) ||
            (task->effective_priority < current->effective_priority));
}

bool hr_queue_init_static(hr_queue_t *queue,
                          const char *name,
                          void *storage,
                          size_t capacity,
                          size_t item_size)
{
    if ((queue == (hr_queue_t *)0) || (name == (const char *)0) ||
        (storage == (void *)0) || (capacity == 0U) || (item_size == 0U) ||
        (capacity > (SIZE_MAX / item_size)))
    {
        return false;
    }
    queue->name = name;
    queue->storage = (uint8_t *)storage;
    queue->capacity = capacity;
    queue->item_size = item_size;
    queue->head = 0U;
    queue->tail = 0U;
    queue->count = 0U;
    queue->send_count = 0U;
    queue->receive_count = 0U;
    queue->send_timeout_count = 0U;
    queue->receive_timeout_count = 0U;
    hr_wait_list_init(&queue->send_waiters);
    hr_wait_list_init(&queue->receive_waiters);
    return true;
}

hr_wait_result_t hr_queue_send(hr_queue_t *queue,
                               const void *item,
                               uint32_t timeout_ticks)
{
    hr_task_t *receiver;
    hr_task_t *current;
    hr_wait_result_t result;
    const hr_irq_state_t state = hr_critical_enter();

    if ((queue == (hr_queue_t *)0) || (item == (const void *)0))
    {
        hr_critical_exit(state);
        return HR_WAIT_CANCELLED;
    }

    receiver = hr_wait_list_front(&queue->receive_waiters);
    if (receiver != (hr_task_t *)0)
    {
        if (receiver->wait_receive_buffer == (void *)0)
        {
            hr_critical_exit(state);
            return HR_WAIT_CANCELLED;
        }
        (void)memcpy(receiver->wait_receive_buffer, item, queue->item_size);
        ++queue->send_count;
        ++queue->receive_count;
        if (!hr_scheduler_wake_task_locked(receiver, HR_WAIT_SUCCESS))
        {
            hr_critical_exit(state);
            return HR_WAIT_CANCELLED;
        }
        hr_critical_exit(state);
        return HR_WAIT_SUCCESS;
    }

    if (queue->count < queue->capacity)
    {
        copy_into_tail(queue, item);
        hr_critical_exit(state);
        return HR_WAIT_SUCCESS;
    }

    if (timeout_ticks == HR_NO_WAIT)
    {
        ++queue->send_timeout_count;
        hr_critical_exit(state);
        return HR_WAIT_TIMEOUT;
    }

    current = hr_scheduler_current();
    if (!hr_task_is_valid(current))
    {
        hr_critical_exit(state);
        return HR_WAIT_CANCELLED;
    }
    current->wait_send_item = item;
    current->wait_receive_buffer = (void *)0;
    if (!hr_scheduler_block_current_locked(queue,
                                           &queue->send_waiters,
                                           HR_WAIT_KIND_QUEUE_SEND,
                                           timeout_ticks))
    {
        current->wait_send_item = (const void *)0;
        hr_critical_exit(state);
        return HR_WAIT_CANCELLED;
    }
    hr_critical_exit(state);
    result = current->wait_result;
    current->wait_send_item = (const void *)0;
    if (result == HR_WAIT_TIMEOUT) { ++queue->send_timeout_count; }
    return result;
}

hr_wait_result_t hr_queue_receive(hr_queue_t *queue,
                                  void *item,
                                  uint32_t timeout_ticks)
{
    hr_task_t *sender;
    hr_task_t *current;
    hr_wait_result_t result;
    const hr_irq_state_t state = hr_critical_enter();

    if ((queue == (hr_queue_t *)0) || (item == (void *)0))
    {
        hr_critical_exit(state);
        return HR_WAIT_CANCELLED;
    }

    if (queue->count > 0U)
    {
        copy_from_head(queue, item);
        sender = hr_wait_list_front(&queue->send_waiters);
        if (sender != (hr_task_t *)0)
        {
            if (sender->wait_send_item == (const void *)0)
            {
                hr_critical_exit(state);
                return HR_WAIT_CANCELLED;
            }
            copy_into_tail(queue, sender->wait_send_item);
            if (!hr_scheduler_wake_task_locked(sender, HR_WAIT_SUCCESS))
            {
                hr_critical_exit(state);
                return HR_WAIT_CANCELLED;
            }
        }
        hr_critical_exit(state);
        return HR_WAIT_SUCCESS;
    }

    if (timeout_ticks == HR_NO_WAIT)
    {
        ++queue->receive_timeout_count;
        hr_critical_exit(state);
        return HR_WAIT_TIMEOUT;
    }

    current = hr_scheduler_current();
    if (!hr_task_is_valid(current))
    {
        hr_critical_exit(state);
        return HR_WAIT_CANCELLED;
    }
    current->wait_receive_buffer = item;
    current->wait_send_item = (const void *)0;
    if (!hr_scheduler_block_current_locked(queue,
                                           &queue->receive_waiters,
                                           HR_WAIT_KIND_QUEUE_RECEIVE,
                                           timeout_ticks))
    {
        current->wait_receive_buffer = (void *)0;
        hr_critical_exit(state);
        return HR_WAIT_CANCELLED;
    }
    hr_critical_exit(state);
    result = current->wait_result;
    current->wait_receive_buffer = (void *)0;
    if (result == HR_WAIT_TIMEOUT) { ++queue->receive_timeout_count; }
    return result;
}

bool hr_queue_send_from_isr(hr_queue_t *queue,
                            const void *item,
                            bool *higher_priority_task_woken)
{
    hr_task_t *receiver = (hr_task_t *)0;
    bool ok = false;
    const hr_irq_state_t state = hr_critical_enter();

    if ((queue != (hr_queue_t *)0) && (item != (const void *)0))
    {
        receiver = hr_wait_list_front(&queue->receive_waiters);
        if ((receiver != (hr_task_t *)0) &&
            (receiver->wait_receive_buffer != (void *)0))
        {
            (void)memcpy(receiver->wait_receive_buffer, item, queue->item_size);
            ++queue->send_count;
            ++queue->receive_count;
            ok = hr_scheduler_wake_task_locked(receiver, HR_WAIT_SUCCESS);
        }
        else if (queue->count < queue->capacity)
        {
            copy_into_tail(queue, item);
            ok = true;
        }
    }

    if (higher_priority_task_woken != (bool *)0)
    {
        *higher_priority_task_woken = ok && higher_than_current(receiver);
    }
    hr_critical_exit(state);
    return ok;
}

bool hr_queue_validate(const hr_queue_t *queue)
{
    if ((queue == (const hr_queue_t *)0) || (queue->name == (const char *)0) ||
        (queue->storage == (uint8_t *)0) || (queue->capacity == 0U) ||
        (queue->item_size == 0U) || (queue->head >= queue->capacity) ||
        (queue->tail >= queue->capacity) || (queue->count > queue->capacity))
    {
        return false;
    }
    return hr_wait_list_validate(&queue->send_waiters) &&
           hr_wait_list_validate(&queue->receive_waiters);
}

bool hr_queue_is_empty(const hr_queue_t *queue)
{
    return (queue != (const hr_queue_t *)0) && (queue->count == 0U);
}

bool hr_queue_is_full(const hr_queue_t *queue)
{
    return (queue != (const hr_queue_t *)0) && (queue->count == queue->capacity);
}

size_t hr_queue_count(const hr_queue_t *queue)
{
    return (queue != (const hr_queue_t *)0) ? queue->count : 0U;
}
