#include "model.h"
#include <stdio.h>

int main(void)
{
    task_t t =
    {
        BLOCKED,
        false,
        true,
        true,
        0
    };
    suspend_task(&t);
    printf("suspended ready=%d wait=%d timeout=%d\n", t.ready_link, t.wait_link, t.timeout_link);
    resume_task(&t);
    printf("resumed ready=%d state=%d\n", t.ready_link, t.state);
    return 0;
}
