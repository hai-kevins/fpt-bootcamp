#include "fsm.h"
#include <stdio.h>
int main(void){const ed_fsm_transition_t t[]={{0U,1U,1U,NULL},{1U,2U,2U,NULL},{2U,3U,0U,NULL}};ed_fsm_t f;ed_event_t e;ed_fsm_init(&f,0U,t,3U,NULL);for(ed_signal_t s=1U;s<=3U;s++){ed_event_init_static(&e,s,0U,0U);if(!ed_fsm_dispatch(&f,&e))return 1;}(void)printf("final=%u handled=%lu\n",f.state,(unsigned long)f.handled_count);return f.state==0U?0:1;}
