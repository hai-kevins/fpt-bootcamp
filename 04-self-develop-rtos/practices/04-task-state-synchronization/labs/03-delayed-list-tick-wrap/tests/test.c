#include "model.h"
#include "test.h"

int main(void)
{
    delayed_t d;
    task_t a =
    {
        "a",
        12,
        false
    },
    b =
    {
        "b",
        10,
        false
    },
    c =
    {
        "c",
        15,
        false
    };
    delayed_init(&d);
    TEST_ASSERT(delayed_insert(&d, &a));
    TEST_ASSERT(delayed_insert(&d, &b));
    TEST_ASSERT(delayed_insert(&d, &c));
    TEST_ASSERT(delayed_validate(&d));
    TEST_ASSERT(delayed_take_due(&d, 9) == 0);
    TEST_ASSERT(delayed_take_due(&d, 10) == &b);
    TEST_ASSERT(delayed_take_due(&d, 12) == &a);
    TEST_ASSERT(tick_reached(1U, 0xFFFFFFFFU));
    TEST_ASSERT(tick_before(0xFFFFFFFFU, 1U));
    TEST_PASS("delayed list and wrap");
    return 0;
}
