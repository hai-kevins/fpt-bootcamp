#include "task_registry.h"
#include "test.h"

static void e(void *a)
{
    (void) a;
}

int main(void)
{
    rtos_task_t a =
    {
        0
    },
    b =
    {
        0
    };
    uint32_t sa[64] __attribute__((aligned(8))), sb[64] __attribute__((aligned(8)));
    rtos_task_registry_init();
    rtos_task_create_static(&a, "alpha", 0, 1, e, 0, sa, 64);
    rtos_task_create_static(&b, "beta", 1, 2, e, 0, sb, 64);
    TEST_ASSERT(rtos_task_registry_add(&a));
    TEST_ASSERT(rtos_task_registry_add(&b));
    TEST_ASSERT(!rtos_task_registry_add(&a));
    TEST_ASSERT(rtos_task_registry_count() == 2);
    TEST_ASSERT(rtos_task_registry_find("beta") == &b);
    TEST_ASSERT(rtos_task_registry_validate());
    TEST_PASS("all-task registry");
    return 0;
}
