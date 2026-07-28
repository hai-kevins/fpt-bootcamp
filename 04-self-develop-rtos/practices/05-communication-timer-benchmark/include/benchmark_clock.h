#ifndef HR_BENCHMARK_CLOCK_H
#define HR_BENCHMARK_CLOCK_H

#include <stdbool.h>
#include <stdint.h>

void hr_benchmark_clock_init(void);
uint32_t hr_benchmark_now(void);
uint32_t hr_benchmark_elapsed(uint32_t start, uint32_t end);
uint32_t hr_benchmark_timestamp_overhead(void);
bool hr_benchmark_clock_available(void);

#endif
