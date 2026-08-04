#include "model.h"
#include "test.h"

int main(void)
{
    state_t s = ST_CREATED;
    TEST_ASSERT(transition_apply(&s, ST_READY));
    TEST_ASSERT(transition_apply(&s, ST_RUNNING));
    TEST_ASSERT(transition_apply(&s, ST_BLOCKED));
    TEST_ASSERT(!transition_apply(&s, ST_RUNNING));
    TEST_ASSERT(s == ST_BLOCKED);
    TEST_ASSERT(transition_apply(&s, ST_SUSPENDED));
    TEST_ASSERT(transition_apply(&s, ST_READY));
    TEST_PASS("task state machine");
    return 0;
}
