#include "policy.h"
#include <assert.h>
#include <stdio.h>

int main(void)
{
    policy_scheduler_t scheduler;
    policy_task_t a =
    {
        .id = 1U,
        .priority = 2U,
        .queued = false
    };
    policy_task_t b =
    {
        .id = 2U,
        .priority = 2U,
        .queued = false
    };
    policy_task_t high =
    {
        .id = 3U,
        .priority = 0U,
        .queued = false
    };
    policy_task_t invalid =
    {
        .id = 4U,
        .priority = 9U,
        .queued = false
    };

    policy_init(&scheduler);
    assert(policy_select(&scheduler) == NULL);
    assert(!policy_add(&scheduler, &invalid));
    assert(policy_add(&scheduler, &a));
    assert(!policy_add(&scheduler, &a));
    assert(policy_add(&scheduler, &b));
    assert(policy_select(&scheduler) == &a);
    assert(policy_rotate(&scheduler, 2U));
    assert(policy_select(&scheduler) == &b);
    assert(policy_add(&scheduler, &high));
    assert(policy_select(&scheduler) == &high);
    assert(policy_remove(&scheduler, &high));
    assert(policy_select(&scheduler) == &b);
    assert(policy_remove(&scheduler, &b));
    assert(policy_select(&scheduler) == &a);
    assert(policy_remove(&scheduler, &a));
    assert(policy_select(&scheduler) == NULL);
    assert(scheduler.bitmap == 0U);

    puts("Lab 05 tests: PASS");
    return 0;
}
