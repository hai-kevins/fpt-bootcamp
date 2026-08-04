#include "model.h"
#include <stdio.h>

int main(void)
{
    task_t t =
    {
        RUNNING,
        true,
        1
    };
    printf("running ready=%d\n", t.in_ready);
    block_task(&t);
    printf("blocked ready=%d\n", t.in_ready);
    wake_task(&t);
    printf("woken ready=%d\n", t.in_ready);
    return 0;
}
