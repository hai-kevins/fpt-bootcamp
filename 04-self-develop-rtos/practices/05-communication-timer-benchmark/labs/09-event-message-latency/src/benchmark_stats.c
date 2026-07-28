#include "benchmark_stats.h"

void hr_benchmark_stats_init(hr_benchmark_stats_t *stats,
                             const uint32_t *bin_upper,
                             size_t bin_count)
{
    if (stats == (hr_benchmark_stats_t *)0) { return; }
    stats->count = 0U;
    stats->minimum = 0U;
    stats->maximum = 0U;
    stats->sum = 0U;
    stats->lost_samples = 0U;
    for (size_t i = 0U; i < HR_BENCHMARK_BIN_COUNT; ++i)
    {
        stats->bin_upper[i] = ((bin_upper != (const uint32_t *)0) && (i < bin_count)) ?
                              bin_upper[i] : UINT32_MAX;
        stats->bin_count[i] = 0U;
    }
    stats->bin_count[HR_BENCHMARK_BIN_COUNT] = 0U;
}

void hr_benchmark_stats_add(hr_benchmark_stats_t *stats, uint32_t sample)
{
    size_t bin = HR_BENCHMARK_BIN_COUNT;
    if (stats == (hr_benchmark_stats_t *)0) { return; }
    if (sample > (UINT32_MAX - stats->sum))
    {
        ++stats->lost_samples;
        return;
    }
    if (stats->count == 0U)
    {
        stats->minimum = sample;
        stats->maximum = sample;
    }
    else
    {
        if (sample < stats->minimum) { stats->minimum = sample; }
        if (sample > stats->maximum) { stats->maximum = sample; }
    }
    for (size_t i = 0U; i < HR_BENCHMARK_BIN_COUNT; ++i)
    {
        if (sample <= stats->bin_upper[i]) { bin = i; break; }
    }
    ++stats->bin_count[bin];
    stats->sum += sample;
    ++stats->count;
}

uint32_t hr_benchmark_stats_average(const hr_benchmark_stats_t *stats)
{
    return ((stats != (const hr_benchmark_stats_t *)0) && (stats->count > 0U)) ?
           (uint32_t)(stats->sum / stats->count) : 0U;
}

bool hr_benchmark_stats_validate(const hr_benchmark_stats_t *stats)
{
    uint32_t total = 0U;
    if (stats == (const hr_benchmark_stats_t *)0) { return false; }
    for (size_t i = 0U; i <= HR_BENCHMARK_BIN_COUNT; ++i) { total += stats->bin_count[i]; }
    if (total != stats->count) { return false; }
    if ((stats->count > 0U) && (stats->minimum > stats->maximum)) { return false; }
    return true;
}
