#include "blocking_queue_model.h"

void model_init(model_queue_t *q)
{
    q->head = q->tail = q->count = 0;
    q->send_waiters = q->receive_waiters = 0;
}

model_result_t model_send(model_queue_t *q, int v, bool wait)
{
    if (q->count == 2)
    {
        if (wait)
        {
            q->send_waiters++;
            return MODEL_BLOCKED;
        }
        return MODEL_WOULD_BLOCK;
    }
    q->storage[q->tail] = v;
    q->tail = (q->tail + 1) % 2;
    q->count++;
    if (q->receive_waiters)
    {
        q->receive_waiters--;
    }
    return MODEL_OK;
}

model_result_t model_receive(model_queue_t *q, int *v, bool wait)
{
    if (q->count == 0)
    {
        if (wait)
        {
            q->receive_waiters++;
            return MODEL_BLOCKED;
        }
        return MODEL_WOULD_BLOCK;
    }
    *v = q->storage[q->head];
    q->head = (q->head + 1) % 2;
    q->count--;
    if (q->send_waiters)
    {
        q->send_waiters--;
    }
    return MODEL_OK;
}

bool model_validate(const model_queue_t *q)
{
    return q && q->head < 2 && q->tail < 2 && q->count <= 2;
}
