#include "ready_queue.h"
#include "task_registry.h"
#include "test.h"
static void e(void*a){(void)a;}
int main(void){rtos_task_t t={0};uint32_t s[64]__attribute__((aligned(8)));rtos_task_registry_init();rtos_ready_init();rtos_task_create_static(&t,"dual",0,1,e,0,s,64);TEST_ASSERT(rtos_task_registry_add(&t));TEST_ASSERT(rtos_ready_insert(&t));TEST_ASSERT(t.all_task_node.owner!=0&&t.ready_node.owner!=0);TEST_ASSERT(t.all_task_node.owner!=t.ready_node.owner);TEST_ASSERT(rtos_task_registry_validate());TEST_ASSERT(rtos_ready_validate());TEST_PASS("TCB multiple lists");return 0;}