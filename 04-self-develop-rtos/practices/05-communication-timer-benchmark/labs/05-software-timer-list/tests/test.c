#include "timer_model.h"
#include <assert.h>
#include <stdio.h>
int main(void){timer_list_t l;timer_model_t a={0},b={0};timer_list_init(&l);assert(timer_start(&l,&a,100,20,0));assert(timer_start(&l,&b,100,10,5));assert(timer_take_due(&l,109)==0);assert(timer_take_due(&l,110)==&b);timer_complete(&l,&b,110);assert(timer_take_due(&l,115)==&b);timer_complete(&l,&b,116);assert(timer_take_due(&l,120)==&a);assert(timer_validate(&l));puts("[PASS] software timer list");return 0;}
