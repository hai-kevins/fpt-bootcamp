#include "model.h"
#include "test.h"

int main(void)
{
    periodic_t p;
    periodic_init(&p, 0, 10);
    TEST_ASSERT(delay_until(&p, 2) == 8);
    TEST_ASSERT(p.next == 10);
    TEST_ASSERT(delay_until(&p, 12) == 8);
    TEST_ASSERT(p.next == 20);
    TEST_ASSERT(relative_next(12, 10) == 22);
    TEST_ASSERT(delay_until(&p, 35) == 0);
    TEST_ASSERT(p.misses == 1);
    TEST_PASS("delay until");
    return 0;
}
