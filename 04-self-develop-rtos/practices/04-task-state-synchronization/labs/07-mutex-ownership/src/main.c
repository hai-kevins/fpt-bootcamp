#include "model.h"
#include <stdio.h>

int main(void)
{
    mutex_t m;
    task_t low =
    {
        "low",
        2,
        false
    },
    high =
    {
        "high",
        0,
        false
    };
    mutex_init(&m);
    mutex_lock(&m, &low, true);
    mutex_lock(&m, &high, true);
    printf("owner=%s high_blocked=%d\n", m.owner->name, high.blocked);
    mutex_unlock(&m, &low);
    printf("owner=%s high_blocked=%d\n", m.owner->name, high.blocked);
    return 0;
}
