#include "task.h"
#include "test.h"

static void worker(void *a)
{
    (void) a;
}

int main(void)
{
    rtos_task_t t =
    {
        0
    };
    uint32_t s[64] __attribute__((aligned(8)));
    TEST_ASSERT(!rtos_task_create_static(0, "x", 0, 0, worker, 0, s, 64));
    TEST_ASSERT(!rtos_task_create_static(&t, 0, 0, 0, worker, 0, s, 64));
    TEST_ASSERT(!rtos_task_create_static(&t, "x", 0, 0, 0, 0, s, 64));
    TEST_ASSERT(!rtos_task_create_static(&t, "x", 0, 0, worker, 0, s, 20));
    TEST_ASSERT(rtos_task_create_static(&t, "x", 0, 2, worker, (void *)0x1234, s, 64));
    TEST_ASSERT(t.state == RTOS_TASK_CREATED && t.magic == RTOS_TASK_MAGIC);
    TEST_ASSERT(!rtos_task_create_static(&t, "x", 0, 2, worker, 0, s, 64));
    TEST_PASS("static task creation");
    return 0;
}
