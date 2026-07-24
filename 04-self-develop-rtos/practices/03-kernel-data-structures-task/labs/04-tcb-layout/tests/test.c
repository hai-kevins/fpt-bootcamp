#include "task_layout.h"
#include "test.h"
int main(void){TEST_ASSERT(offsetof(task_t,saved_sp)==0U);TEST_ASSERT(offsetof(task_t,ready_node)!=offsetof(task_t,all_task_node));TEST_ASSERT(sizeof(((task_t*)0)->base_priority)==1U);TEST_PASS("TCB layout");return 0;}