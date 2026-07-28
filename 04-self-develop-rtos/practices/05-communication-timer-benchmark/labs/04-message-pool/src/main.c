#include "message_pool.h"
#include <stdint.h>
#include <stdio.h>
int main(void){hr_message_pool_t p;uint8_t mem[3*24] __attribute__((aligned(8)));uint8_t map[3];hr_message_pool_init_static(&p,"demo",mem,24,3,map);for(int i=0;i<4;i++)printf("alloc[%d]=%p free=%zu\n",i,hr_message_pool_alloc(&p),p.free_count);return 0;}
