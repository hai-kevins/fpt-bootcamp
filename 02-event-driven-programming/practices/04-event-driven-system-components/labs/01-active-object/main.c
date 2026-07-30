#include "active_object.h"
#include "event_pool.h"
#include "scheduler.h"
#include <stdio.h>
static unsigned handled;
static void handler(ed_active_object_t *o,const ed_event_t *e){(void)o;(void)e;handled++;}
int main(void){ed_event_pool_t p;ed_scheduler_t s;ed_active_object_t ao;ed_event_t e;
ed_event_pool_init(&p);ed_scheduler_init(&s,&p);ed_event_init_static(&e,1U,0U,1U);
if(!ed_active_object_init(&ao,1U,1U,"ao",4U,handler,NULL)||!ed_scheduler_register(&s,&ao)||!ed_active_object_post(&ao,&e))return 1;
(void)ed_scheduler_run_once(&s);(void)printf("handled=%u\n",handled);return handled==1U?0:1;}
