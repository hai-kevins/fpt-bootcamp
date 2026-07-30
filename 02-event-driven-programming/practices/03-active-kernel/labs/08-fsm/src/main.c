#include <assert.h>
#include <stdint.h>
#include <stdio.h>
enum {IDLE,RUNNING,PAUSED,ERROR}; enum {START=1,PAUSE,RESUME,STOP,FAIL,RESET};
static uint8_t state=IDLE;
static void dispatch(uint16_t sig){switch(state){case IDLE:if(sig==START)state=RUNNING;else if(sig==FAIL)state=ERROR;break;case RUNNING:if(sig==PAUSE)state=PAUSED;else if(sig==STOP)state=IDLE;else if(sig==FAIL)state=ERROR;break;case PAUSED:if(sig==RESUME)state=RUNNING;else if(sig==STOP)state=IDLE;else if(sig==FAIL)state=ERROR;break;case ERROR:if(sig==RESET)state=IDLE;break;}}
int main(void){dispatch(START);assert(state==RUNNING);dispatch(PAUSE);assert(state==PAUSED);dispatch(RESUME);assert(state==RUNNING);dispatch(FAIL);assert(state==ERROR);dispatch(RESET);assert(state==IDLE);puts("FSM PASS");return 0;}
