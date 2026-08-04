#ifndef HR_QUEUE_H
#define HR_QUEUE_H

#include "list.h"
#include "task.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct
{
    const char *name;
    uint8_t *storage;
    size_t capacity;
    size_t item_size;
    size_t head;
    size_t tail;
    size_t count;
    uint32_t send_count;
    uint32_t receive_count;
    uint32_t send_timeout_count;
    uint32_t receive_timeout_count;
    hr_list_t send_waiters;
    hr_list_t receive_waiters;
} hr_queue_t;

bool hr_queue_init_static(hr_queue_t *queue, const char *name, void *storage, size_t capacity, size_t item_size);
hr_wait_result_t hr_queue_send(hr_queue_t *queue, const void *item, uint32_t timeout_ticks);
hr_wait_result_t hr_queue_receive(hr_queue_t *queue, void *item, uint32_t timeout_ticks);
bool hr_queue_send_from_isr(hr_queue_t *queue, const void *item, bool *higher_priority_task_woken);
bool hr_queue_validate(const hr_queue_t *queue);
bool hr_queue_is_empty(const hr_queue_t *queue);
bool hr_queue_is_full(const hr_queue_t *queue);
size_t hr_queue_count(const hr_queue_t *queue);

#endif
