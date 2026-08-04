#include "model.h"
#include "test.h"

int main(void)
{
    sem_t s;
    task_t low =
    {
        "low",
        2,
        false,
        0
    },
    high =
    {
        "high",
        0,
        false,
        0
    },
    mid =
    {
        "mid",
        1,
        false,
        0
    };
    task_t *w = 0;
    sem_init(&s, 0, 2);
    TEST_ASSERT(!sem_take(&s, &low, true));
    TEST_ASSERT(!sem_take(&s, &high, true));
    TEST_ASSERT(!sem_take(&s, &mid, true));
    TEST_ASSERT(sem_validate(&s));
    TEST_ASSERT(sem_give(&s, &w) && w == &high && s.count == 0);
    TEST_ASSERT(sem_give(&s, &w) && w == &mid);
    TEST_ASSERT(sem_give(&s, &w) && w == &low);
    TEST_ASSERT(sem_give(&s, &w) && s.count == 1);
    TEST_PASS("semaphore");
    return 0;
}
