#include "event_pool.h"
#include "test_assert.h"
#include "test_cases.h"

bool test_pool_allocate_release(void)
{
    event_pool_t pool;

    TEST_ASSERT_TRUE(event_pool_init(&pool, 2U));

    event_t *first = event_pool_allocate(&pool, 1U);
    event_t *second = event_pool_allocate(&pool, 2U);

    TEST_ASSERT_NE(NULL, first);
    TEST_ASSERT_NE(NULL, second);
    TEST_ASSERT_EQ(2U, pool.used_count);
    TEST_ASSERT_EQ(NULL, event_pool_allocate(&pool, 3U));
    TEST_ASSERT_EQ(1U, pool.allocation_failures);

    TEST_ASSERT_TRUE(event_pool_release(&pool, first));
    TEST_ASSERT_TRUE(event_pool_release(&pool, second));
    TEST_ASSERT_EQ(0U, pool.used_count);
    return true;
}

bool test_pool_reference_count(void)
{
    event_pool_t pool;

    TEST_ASSERT_TRUE(event_pool_init(&pool, 1U));
    event_t *event = event_pool_allocate(&pool, 1U);

    TEST_ASSERT_NE(NULL, event);
    TEST_ASSERT_EQ(1U, event_pool_reference_count(&pool, event));

    TEST_ASSERT_TRUE(event_pool_retain(&pool, event));
    TEST_ASSERT_EQ(2U, event_pool_reference_count(&pool, event));

    TEST_ASSERT_TRUE(event_pool_release(&pool, event));
    TEST_ASSERT_EQ(1U, event_pool_reference_count(&pool, event));

    TEST_ASSERT_TRUE(event_pool_release(&pool, event));
    TEST_ASSERT_EQ(0U, pool.used_count);
    return true;
}
