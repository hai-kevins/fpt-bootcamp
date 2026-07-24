#include "model.h"
#include "test.h"
int main(void){task_t t={BLOCKED,false,true,true,0};TEST_ASSERT(suspend_task(&t));TEST_ASSERT(validate_task(&t));TEST_ASSERT(!timeout_wake(&t));TEST_ASSERT(!suspend_task(&t));TEST_ASSERT(resume_task(&t));TEST_ASSERT(validate_task(&t));TEST_ASSERT(!resume_task(&t));TEST_PASS("suspend resume");return 0;}
