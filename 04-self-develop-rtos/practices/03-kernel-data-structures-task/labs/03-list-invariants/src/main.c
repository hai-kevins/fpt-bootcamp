#include "list.h"
#include <stdio.h>
int main(void){rtos_list_t l;rtos_list_node_t a,b;rtos_list_init(&l);rtos_list_node_init(&a);rtos_list_node_init(&b);rtos_list_push_back(&l,&a);rtos_list_push_back(&l,&b);printf("count=%zu owner_a=%s valid=%s\n",l.count,a.owner==&l?"list":"none",rtos_list_validate(&l)?"yes":"no");return 0;}