#include "model.h"
#include <stdio.h>
int main(void){state_t s=ST_CREATED;state_t path[]={ST_READY,ST_RUNNING,ST_BLOCKED,ST_READY,ST_SUSPENDED,ST_READY};for(unsigned i=0;i<sizeof(path)/sizeof(path[0]);++i){printf("%s -> %s : %s\n",state_name(s),state_name(path[i]),transition_apply(&s,path[i])?"OK":"REJECT");}return 0;}
