#include "task_registry.h"
#include <stdio.h>
static void e(void*a){(void)a;}
int main(void){hr_task_t a={0},b={0};uint32_t sa[64]__attribute__((aligned(8))),sb[64]__attribute__((aligned(8)));hr_task_registry_init();hr_task_create_static(&a,"alpha",0,1,e,0,sa,64);hr_task_create_static(&b,"beta",1,2,e,0,sb,64);hr_task_registry_add(&a);hr_task_registry_add(&b);for(size_t i=0;i<hr_task_registry_count();i++)printf("%s\n",hr_task_registry_at(i)->name);return 0;}