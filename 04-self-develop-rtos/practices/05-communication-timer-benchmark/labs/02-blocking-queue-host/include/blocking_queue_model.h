#ifndef BLOCKING_QUEUE_MODEL_H
#define BLOCKING_QUEUE_MODEL_H
#include <stdbool.h>
#include <stddef.h>
typedef enum{MODEL_OK,MODEL_WOULD_BLOCK,MODEL_BLOCKED,MODEL_TIMEOUT}model_result_t;
typedef struct{int storage[2];size_t head,tail,count;unsigned send_waiters,receive_waiters;}model_queue_t;
void model_init(model_queue_t*);model_result_t model_send(model_queue_t*,int,bool);model_result_t model_receive(model_queue_t*,int*,bool);bool model_validate(const model_queue_t*);
#endif
