#include <assert.h>
#include <stdint.h>
#include <stdio.h>
static uint32_t period=1000U,deadline,count;
static void start(uint32_t now,uint32_t p){period=p;deadline=now+p;}
static void process(uint32_t now){while((int32_t)(now-deadline)>=0){count++;deadline+=period;}}
int main(void){start(0U,1000U);process(999U);assert(count==0U);process(1000U);assert(count==1U);start(1000U,100U);process(1300U);assert(count==4U);printf("heartbeat count=%lu period=%lu\n",(unsigned long)count,(unsigned long)period);return 0;}
