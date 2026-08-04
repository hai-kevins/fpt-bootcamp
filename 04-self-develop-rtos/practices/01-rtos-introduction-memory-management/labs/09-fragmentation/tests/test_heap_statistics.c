#include "test.h"
#include "heap.h"
#include "heap_stats.h"
#include <stdint.h>

int test_statistics_and_fragmentation(void)
{
    uint8_t memory[1024];
    heap_stats_t stats;
    void *a;
    void *b;
    void *c;
    void *d;
    TEST_ASSERT(heap_init(memory, sizeof(memory)) == HEAP_STATUS_OK);
    a = heap_alloc(32U);
    b = heap_alloc(96U);
    c = heap_alloc(32U);
    d = heap_alloc(96U);
    TEST_ASSERT(a && b && c && d);
    TEST_ASSERT(heap_free(b) == HEAP_STATUS_OK);
    TEST_ASSERT(heap_free(d) == HEAP_STATUS_OK);
    TEST_ASSERT(heap_get_stats(&stats) == HEAP_STATUS_OK);
    TEST_ASSERT(stats.free_block_count >= 2U);
    TEST_ASSERT(stats.free_payload_bytes >= stats.largest_free_block);
    TEST_ASSERT(heap_external_fragmentation_per_mille(&stats) > 0U);
    TEST_ASSERT(heap_validate());
    return 0;
}
