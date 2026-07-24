#include "model.h"
#include "test.h"
int main(void){task_t t={RUNNING,true,1};TEST_ASSERT(validate_task(&t));TEST_ASSERT(block_task(&t));TEST_ASSERT(validate_task(&t));TEST_ASSERT(wake_task(&t));TEST_ASSERT(!wake_task(&t));TEST_ASSERT(validate_task(&t));TEST_ASSERT(suspend_task(&t));TEST_ASSERT(!wake_task(&t));TEST_ASSERT(resume_task(&t));TEST_PASS("efficient blocking");return 0;}
