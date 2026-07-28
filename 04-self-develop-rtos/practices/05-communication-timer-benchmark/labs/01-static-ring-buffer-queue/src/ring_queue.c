#include "ring_queue.h"
#include <string.h>
bool ring_queue_init(ring_queue_t*q,void*s,size_t c,size_t z){if(!q||!s||!c||!z)return false;q->storage=s;q->capacity=c;q->item_size=z;q->head=q->tail=q->count=0;return true;}
bool ring_queue_send(ring_queue_t*q,const void*i){if(!q||!i||q->count==q->capacity)return false;memcpy(q->storage+q->tail*q->item_size,i,q->item_size);q->tail=(q->tail+1)%q->capacity;q->count++;return true;}
bool ring_queue_receive(ring_queue_t*q,void*i){if(!q||!i||q->count==0)return false;memcpy(i,q->storage+q->head*q->item_size,q->item_size);q->head=(q->head+1)%q->capacity;q->count--;return true;}
bool ring_queue_validate(const ring_queue_t*q){return q&&q->storage&&q->capacity&&q->item_size&&q->head<q->capacity&&q->tail<q->capacity&&q->count<=q->capacity;}
