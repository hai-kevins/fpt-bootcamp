#include <stdlib.h>
void hr_panic(int code,unsigned detail){(void)code;(void)detail;abort();}
void hr_assert_failed(unsigned line){(void)line;abort();}
