#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

typedef struct
{
    uint16_t items[3];
    size_t head;
    size_t tail;
    size_t count;
    uint32_t overflow;
} queue_t;

static bool post(queue_t *queue, uint16_t value)
{
    if (queue->count == 3U)
    {
        queue->overflow++;
        return false;
    }

    queue->items[queue->tail] = value;
    queue->tail = (queue->tail + 1U) % 3U;
    queue->count++;
    return true;
}

static bool get(queue_t *queue, uint16_t *value)
{
    if (queue->count == 0U)
    {
        return false;
    }

    *value = queue->items[queue->head];
    queue->head = (queue->head + 1U) % 3U;
    queue->count--;
    return true;
}

int main(void)
{
    queue_t queue = {0};
    uint16_t value = 0U;

    const bool empty_pass = !get(&queue, &value);

    (void)post(&queue, 1U);
    (void)post(&queue, 2U);
    (void)post(&queue, 3U);

    const bool full_pass = !post(&queue, 4U);

    bool fifo_pass = get(&queue, &value) && (value == 1U);
    fifo_pass = fifo_pass && get(&queue, &value) && (value == 2U);

    (void)post(&queue, 4U);
    (void)post(&queue, 5U);

    bool wrap_pass = get(&queue, &value) && (value == 3U);
    wrap_pass = wrap_pass && get(&queue, &value) && (value == 4U);
    wrap_pass = wrap_pass && get(&queue, &value) && (value == 5U);

    (void)printf(
        "empty=%s fifo=%s full=%s wrap=%s overflow=%lu\n",
        empty_pass ? "PASS" : "FAIL",
        fifo_pass ? "PASS" : "FAIL",
        full_pass ? "PASS" : "FAIL",
        wrap_pass ? "PASS" : "FAIL",
        (unsigned long)queue.overflow
    );

    return (empty_pass && fifo_pass && full_pass &&
            wrap_pass && (queue.overflow == 1U)) ? 0 : 1;
}
