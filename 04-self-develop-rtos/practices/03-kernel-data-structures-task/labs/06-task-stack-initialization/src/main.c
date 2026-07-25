#include "task.h"
#include <stdio.h>
static void entry(void*a){(void)a;}
int main(void){rtos_task_t t={0};uint32_t s[80] __attribute__((aligned(8)));rtos_task_create_static(&t,"stack-demo",0,1,entry,(void*)0x12345678,s,80);printf("saved_sp=%p guard=%s unused=%zu\n",(void*)t.saved_sp,rtos_task_stack_guard_ok(&t)?"OK":"FAIL",rtos_task_stack_unused_words(&t));for(size_t i=0;i<16;i++)printf("frame[%zu]=0x%08X\n",i,(unsigned)t.saved_sp[i]);return 0;}