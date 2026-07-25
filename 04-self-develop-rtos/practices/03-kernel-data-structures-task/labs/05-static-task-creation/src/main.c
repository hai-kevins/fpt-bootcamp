#include "task.h"
#include <stdio.h>
static void worker(void*a){(void)a;}
int main(void){rtos_task_t t={0};uint32_t stack[64] __attribute__((aligned(8)));printf("create=%s\n",rtos_task_create_static(&t,"worker",1,2,worker,0,stack,64)?"OK":"FAIL");printf("name=%s state=%u priority=%u saved_sp=%p\n",t.name,(unsigned)t.state,t.effective_priority,(void*)t.saved_sp);return 0;}