#include "task_layout.h"
#include <stdio.h>
int main(void){printf("sizeof(task_t)=%zu\n",sizeof(task_t));printf("saved_sp=%zu ready_node=%zu all_task_node=%zu\n",offsetof(task_t,saved_sp),offsetof(task_t,ready_node),offsetof(task_t,all_task_node));return 0;}