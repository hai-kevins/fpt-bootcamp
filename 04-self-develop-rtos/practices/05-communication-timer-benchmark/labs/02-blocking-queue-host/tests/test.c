#include "blocking_queue_model.h"
#include <assert.h>
#include <stdio.h>

int main(void)
{
    model_queue_t q;
    int v;
    model_init(&q);
    assert(model_receive(&q, &v, false) == MODEL_WOULD_BLOCK);
    assert(model_receive(&q, &v, true) == MODEL_BLOCKED);
    assert(q.receive_waiters == 1);
    assert(model_send(&q, 7, false) == MODEL_OK);
    assert(q.receive_waiters == 0);
    assert(model_receive(&q, &v, false) == MODEL_OK && v == 7);
    assert(model_send(&q, 1, false) == MODEL_OK);
    assert(model_send(&q, 2, false) == MODEL_OK);
    assert(model_send(&q, 3, true) == MODEL_BLOCKED);
    assert(q.send_waiters == 1);
    assert(model_receive(&q, &v, false) == MODEL_OK && v == 1);
    assert(q.send_waiters == 0);
    assert(model_validate(&q));
    puts("[PASS] blocking queue model");
    return 0;
}
