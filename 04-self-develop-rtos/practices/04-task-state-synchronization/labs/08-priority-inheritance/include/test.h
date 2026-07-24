#ifndef TEST_H
#define TEST_H
#include <stdio.h>
#include <stdlib.h>
#define TEST_ASSERT(c) do { if (!(c)) { fprintf(stderr,"FAIL %s:%d: %s\n",__FILE__,__LINE__,#c); exit(1); } } while (0)
#define TEST_PASS(n) do { printf("PASS: %s\n", n); } while (0)
#endif
