#include <assert.h>
#include <stdio.h>
#include <string.h>
static unsigned posted;
static void post(const char *cmd){if(strcmp(cmd,"led on")==0||strcmp(cmd,"led off")==0||strcmp(cmd,"state")==0){posted++;printf("post: %s\n",cmd);}else puts("unknown");}
int main(void){post("led on");post("state");post("bad");assert(posted==2U);return 0;}
