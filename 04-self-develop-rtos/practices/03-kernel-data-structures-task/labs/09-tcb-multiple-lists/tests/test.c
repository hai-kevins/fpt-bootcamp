#include "ready_queue.h"
#include "task_registry.h"
#include "test.h"
static void e(void*a){(void)a;}
int main(void){hr_task_t t={0};uint32_t s[64]__attribute__((aligned(8)));hr_task_registry_init();hr_ready_init();hr_task_create_static(&t,"dual",0,1,e,0,s,64);TEST_ASSERT(hr_task_registry_add(&t));TEST_ASSERT(hr_ready_insert(&t));TEST_ASSERT(t.all_task_node.owner!=0&&t.ready_node.owner!=0);TEST_ASSERT(t.all_task_node.owner!=t.ready_node.owner);TEST_ASSERT(hr_task_registry_validate());TEST_ASSERT(hr_ready_validate());TEST_PASS("TCB multiple lists");return 0;}