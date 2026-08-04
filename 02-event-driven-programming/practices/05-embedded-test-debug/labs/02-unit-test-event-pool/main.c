#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

typedef struct
{
    bool used[4];
    size_t current;
    size_t high_water;
} pool_t;

static int allocate(pool_t *pool)
{
    for (size_t i = 0U; i < 4U; i++)
    {
        if (!pool->used[i])
        {
            pool->used[i] = true;
            pool->current++;
            if (pool->current > pool->high_water)
            {
                pool->high_water = pool->current;
            }
            return (int) i;
        }
    }

    return -1;
}

static bool release(pool_t *pool, int id)
{
    if ((id < 0) || (id >= 4) || !pool->used[(size_t) id])
    {
        return false;
    }

    pool->used[(size_t) id] = false;
    pool->current--;
    return true;
}

int main(void)
{
    pool_t pool =
    {
        0
    };
    int ids[4];

    bool allocate_pass = true;
    for (size_t i = 0U; i < 4U; i++)
    {
        ids[i] = allocate(&pool);
        allocate_pass = allocate_pass && (ids[i] >= 0);
    }

    const bool exhaustion_pass = allocate(&pool) == -1;
    const bool free_pass = release(&pool, ids[1]);
    const int reused = allocate(&pool);
    const bool reuse_pass = free_pass && (reused == ids[1]);
    const bool double_free_pass = release(&pool, reused) && !release(&pool, reused);

    for (size_t i = 0U; i < 4U; i++)
    {
        if ((ids[i] != reused) && pool.used[(size_t) ids[i]])
        {
            (void) release(&pool, ids[i]);
        }
    }

    (void) printf("allocate=%s exhaustion=%s reuse=%s ""double_free=%s hwm=%zu\n", allocate_pass ? "PASS" : "FAIL",
        exhaustion_pass ? "PASS" : "FAIL", reuse_pass ? "PASS" : "FAIL", double_free_pass ? "PASS" : "FAIL", pool.high_water);

    return (allocate_pass && exhaustion_pass && reuse_pass && double_free_pass && (pool.current == 0U) && (pool.high_water == 4U)) ? 0 : 1;
}
