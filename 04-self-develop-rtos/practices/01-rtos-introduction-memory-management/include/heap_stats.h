#ifndef MEMORY_EXPLORER_HEAP_STATS_H
#define MEMORY_EXPLORER_HEAP_STATS_H
#include "heap.h"
#include <stdint.h>

uint32_t heap_external_fragmentation_per_mille(const heap_stats_t *stats);
#endif
