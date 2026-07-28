#ifndef HR_BENCHMARK_STATS_H
#define HR_BENCHMARK_STATS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define HR_BENCHMARK_BIN_COUNT (8U)

typedef struct
{
    uint32_t count;
    uint32_t minimum;
    uint32_t maximum;
    uint32_t sum;
    uint32_t lost_samples;
    uint32_t bin_upper[HR_BENCHMARK_BIN_COUNT];
    uint32_t bin_count[HR_BENCHMARK_BIN_COUNT + 1U];
} hr_benchmark_stats_t;

void hr_benchmark_stats_init(hr_benchmark_stats_t *stats,
                             const uint32_t *bin_upper,
                             size_t bin_count);
void hr_benchmark_stats_add(hr_benchmark_stats_t *stats, uint32_t sample);
uint32_t hr_benchmark_stats_average(const hr_benchmark_stats_t *stats);
bool hr_benchmark_stats_validate(const hr_benchmark_stats_t *stats);

#endif
