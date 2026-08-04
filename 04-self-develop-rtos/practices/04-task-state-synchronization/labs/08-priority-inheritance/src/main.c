#include "model.h"
#include <stdio.h>

int main(void)
{
    mutex_t m;
    task_t low =
    {
        "low",
        2,
        2,
        true,
        false
    },
    high =
    {
        "high",
        0,
        0,
        true,
        false
    };
    mutex_init(&m);
    lock_with_pi(&m, &low);
    printf("low effective before=%u\n", low.effective);
    lock_with_pi(&m, &high);
    printf("low effective inherited=%u\n", low.effective);
    unlock_with_pi(&m, &low);
    printf("low restored=%u owner=%s\n", low.effective, m.owner->name);
    return 0;
}
