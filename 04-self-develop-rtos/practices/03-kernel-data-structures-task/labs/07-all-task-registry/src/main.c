#include "task_registry.h"
#include <stdio.h>

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
    rtos_task_registry_add(&a);
    rtos_task_registry_add(&b);
    for (size_t i = 0; i < rtos_task_registry_count(); i++)
    {
        printf("%s\n", rtos_task_registry_at(i)->name);
    }
    return 0;
}
