#include "model.h"
#include "test.h"
int main(void){mutex_t m;task_t a={"A",2,false},b={"B",0,false},c={"C",1,false};mutex_init(&m);TEST_ASSERT(mutex_lock(&m,&a,true));TEST_ASSERT(!mutex_lock(&m,&a,true));TEST_ASSERT(!mutex_lock(&m,&b,true));TEST_ASSERT(!mutex_lock(&m,&c,true));TEST_ASSERT(!mutex_unlock(&m,&c));TEST_ASSERT(mutex_unlock(&m,&a));TEST_ASSERT(m.owner==&b&&!b.blocked);TEST_ASSERT(mutex_validate(&m));TEST_PASS("mutex ownership");return 0;}
