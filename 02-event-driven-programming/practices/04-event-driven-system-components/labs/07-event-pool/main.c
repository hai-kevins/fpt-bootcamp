#include "event_pool.h"
#include <stdio.h>
int main(void){ed_event_pool_t p;ed_event_t*e[ED_EVENT_POOL_CAPACITY];ed_event_pool_init(&p);for(size_t i=0;i<ED_EVENT_POOL_CAPACITY;i++){e[i]=ed_event_pool_allocate(&p,1U,0U,0U);if(e[i]==NULL)return 1;}int full=ed_event_pool_allocate(&p,1U,0U,0U)==NULL;for(size_t i=0;i<ED_EVENT_POOL_CAPACITY;i++)ed_event_pool_release(&p,e[i]);(void)printf("hwm=%zu failures=%lu\n",p.high_water_mark,(unsigned long)p.allocation_failure_count);return(full&&p.used_count==0U)?0:1;}
