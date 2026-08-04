#include "heap.h"
#include "heap_stats.h"
#include <stdint.h>
#include <stdio.h>

int main(void)
{
    uint8_t memory[1024];
    void *a;
    void *b;
    void *c;
    void *d;
    heap_stats_t stats;
    printf("Lab 09 - Fragmentation\n");
    if (heap_init(memory, sizeof(memory)) != HEAP_STATUS_OK)
    {
        return 1;
    }
    a = heap_alloc(24U);
    b = heap_alloc(160U);
    c = heap_alloc(24U);
    d = heap_alloc(160U);
    (void) a;
    (void) c;
    (void) heap_free(b);
    (void) heap_free(d);
    if (heap_get_stats(&stats) != HEAP_STATUS_OK)
    {
        return 1;
    }
    printf("Free=%zu largest=%zu external-frag=%lu/1000 blocks=%zu\n", stats.free_payload_bytes, stats.largest_free_block,
        (unsigned long) heap_external_fragmentation_per_mille(&stats), stats.block_count);
    return heap_validate() ? 0 : 1;
}
