#include <stdlib.h>
void rtos_panic(int code,unsigned detail){(void)code;(void)detail;abort();}
void rtos_assert_failed(unsigned line){(void)line;abort();}
