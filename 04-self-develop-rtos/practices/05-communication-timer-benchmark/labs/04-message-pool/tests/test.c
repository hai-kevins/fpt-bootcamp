#include "message_pool.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
int main(void){hr_message_pool_t p;uint8_t mem[4*32] __attribute__((aligned(8)));uint8_t map[4];void*b[4];assert(hr_message_pool_init_static(&p,"pool",mem,32,4,map));for(int i=0;i<4;i++){b[i]=hr_message_pool_alloc(&p);assert(b[i]);}assert(!hr_message_pool_alloc(&p));assert(hr_message_pool_free(&p,b[2]));assert(!hr_message_pool_free(&p,b[2]));assert(hr_message_pool_alloc(&p)==b[2]);assert(hr_message_pool_validate(&p));puts("[PASS] message pool");return 0;}
