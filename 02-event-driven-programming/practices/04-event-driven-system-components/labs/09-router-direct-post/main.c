#include "router.h"
#include "scheduler.h"
#include <stdio.h>
static unsigned hit;static void h(ed_active_object_t*o,const ed_event_t*e){(void)o;(void)e;hit++;}
int main(void){ed_event_pool_t p;ed_router_t r;ed_scheduler_t s;ed_active_object_t a;ed_event_pool_init(&p);ed_router_init(&r,1U,&p,NULL,NULL);ed_scheduler_init(&s,&p);ed_active_object_init(&a,4U,1U,"a",4U,h,NULL);ed_router_register(&r,&a);ed_scheduler_register(&s,&a);ed_event_t*e=ed_event_pool_allocate(&p,2U,0U,4U);if(e==NULL)return 1;int ok=ed_router_post(&r,1U,e);ed_scheduler_run_until_idle(&s,2U);(void)printf("local=%lu hit=%u\n",(unsigned long)r.local_post_count,hit);return(ok&&hit==1U&&p.used_count==0U)?0:1;}
