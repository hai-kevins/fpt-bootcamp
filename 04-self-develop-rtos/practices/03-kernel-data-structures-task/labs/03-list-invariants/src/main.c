#include "list.h"
#include <stdio.h>
int main(void){hr_list_t l;hr_list_node_t a,b;hr_list_init(&l);hr_list_node_init(&a);hr_list_node_init(&b);hr_list_push_back(&l,&a);hr_list_push_back(&l,&b);printf("count=%zu owner_a=%s valid=%s\n",l.count,a.owner==&l?"list":"none",hr_list_validate(&l)?"yes":"no");return 0;}