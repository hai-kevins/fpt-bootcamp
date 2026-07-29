#ifndef LAB02_EVENT_QUEUE_H
#define LAB02_EVENT_QUEUE_H
#include "event.h"
#include <stdbool.h>
#include <stdint.h>
#define LAB02_QUEUE_CAPACITY (8U)
typedef struct
{
    event_t buffer[LAB02_QUEUE_CAPACITY];
    uint16_t head;
    uint16_t tail;
    uint16_t count;
    uint16_t high_water_mark;
    uint32_t overflow_count;
} event_queue_t;
void event_queue_init(event_queue_t *queue);
bool event_queue_post(event_queue_t *queue, const event_t *event);
bool event_queue_get(event_queue_t *queue, event_t *event);
#endif
