#include "heap_stats.h"

uint32_t heap_external_fragmentation_per_mille(const heap_stats_t *stats)
{
    if ((stats == (const heap_stats_t *)0) || (stats->free_payload_bytes == 0U))
    {
        return 0U;
    }
    return 1000UL - (uint32_t)((stats->largest_free_block * 1000U) / stats->free_payload_bytes);
}
