#include "model.h"
#include "test.h"

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
    med =
    {
        "med",
        1,
        1,
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
    (void) med;
    mutex_init(&m);
    TEST_ASSERT(lock_with_pi(&m, &low));
    TEST_ASSERT(!lock_with_pi(&m, &high));
    TEST_ASSERT(low.effective == 0 && high.blocked);
    TEST_ASSERT(unlock_with_pi(&m, &low));
    TEST_ASSERT(low.effective == 2 && m.owner == &high && high.ready);
    TEST_PASS("priority inheritance");
    return 0;
}
