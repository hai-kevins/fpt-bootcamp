#include "ready_queue.h"
#include "task_registry.h"
#include <stdio.h>
static void e(void*a){(void)a;}
int main(void){hr_task_t t={0};uint32_t s[64]__attribute__((aligned(8)));hr_task_registry_init();hr_ready_init();hr_task_create_static(&t,"dual-member",0,1,e,0,s,64);hr_task_registry_add(&t);hr_ready_insert(&t);printf("registry_owner=%p ready_owner=%p distinct=%s\n",t.all_task_node.owner,t.ready_node.owner,t.all_task_node.owner!=t.ready_node.owner?"yes":"no");return 0;}