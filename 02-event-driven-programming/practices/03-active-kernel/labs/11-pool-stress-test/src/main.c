#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#define POOL 16U
#define QUEUE 8U

int main(void)
{
    uint32_t posted = 0, post_fail = 0, alloc_fail = 0;
    uint8_t used = 0, q = 0, max_used = 0, max_q = 0;
    for (uint32_t i = 0; i < 1000U; i++)
    {
        if (used == POOL)
        {
            alloc_fail++;
        }
        else
        {
            used++;
            if (used > max_used)
            {
                max_used = used;
            }
            if (q == QUEUE)
            {
                post_fail++;
                used--;
            }
            else
            {
                q++;
                posted++;
                if (q > max_q)
                {
                    max_q = q;
                }
            }
        }
        if ((i % 3U) == 0U && q)
        {
            q--;
            used--;
        }
    }
    while (q)
    {
        q--;
        used--;
    }
    assert(used == 0U);
    printf("posted=%lu post_fail=%lu alloc_fail=%lu pool_max=%u queue_max=%u\n", (unsigned long) posted, (unsigned long) post_fail,
        (unsigned long) alloc_fail, max_used, max_q);
    return 0;
}
