#include "blocking_queue_model.h"
#include <stdio.h>
int main(void){model_queue_t q;int value;model_init(&q);printf("receive empty=%d\n",model_receive(&q,&value,true));printf("send wakes receiver=%d\n",model_send(&q,42,false));return 0;}
