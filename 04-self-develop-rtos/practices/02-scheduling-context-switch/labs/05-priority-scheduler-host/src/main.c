#include "policy.h"
#include <stdio.h>

int main(void)
{
    policy_scheduler_t scheduler;
    policy_task_t a =
    {
        .id = 1U,
        .priority = 1U,
        .queued = false
    };
    policy_task_t b =
    {
        .id = 2U,
        .priority = 1U,
        .queued = false
    };
    policy_task_t high =
    {
        .id = 3U,
        .priority = 0U,
        .queued = false
    };

    policy_init(&scheduler);
    (void) policy_add(&scheduler, &a);
    (void) policy_add(&scheduler, &b);
    printf("selected=%u\n", policy_select(&scheduler)->id);
    (void) policy_rotate(&scheduler, 1U);
    printf("after rotate=%u\n", policy_select(&scheduler)->id);
    (void) policy_add(&scheduler, &high);
    printf("after high ready=%u\n", policy_select(&scheduler)->id);
    return 0;
}
