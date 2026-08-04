#include "task.h"
#include "test.h"
#include <stdint.h>

static void entry(void *a)
{
    (void) a;
}

int main(void)
{
    rtos_task_t t =
    {
        0
    };
    uint32_t s[80] __attribute__((aligned(8)));
    void *arg = (void *)(uintptr_t)0x12345678UL;
    TEST_ASSERT(rtos_task_create_static(&t, "x", 0, 1, entry, arg, s, 80));
    TEST_ASSERT(rtos_task_saved_sp_is_valid(&t));
    TEST_ASSERT(rtos_task_stack_guard_ok(&t));
    TEST_ASSERT(t.saved_sp[8] == (uint32_t)(uintptr_t) arg);
    TEST_ASSERT(t.saved_sp[14] == (uint32_t)(uintptr_t) entry);
    TEST_ASSERT(t.saved_sp[15] == 0x01000000UL);
    s[0] = 0;
    TEST_ASSERT(!rtos_task_stack_guard_ok(&t));
    TEST_PASS("task stack frame");
    return 0;
}
