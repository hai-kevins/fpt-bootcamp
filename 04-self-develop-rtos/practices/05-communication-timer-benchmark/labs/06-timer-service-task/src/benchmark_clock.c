#include "benchmark_clock.h"
#include <stdint.h>

#define DEMCR      (*(volatile uint32_t *)(uintptr_t)0xE000EDFCUL)
#define DWT_CTRL   (*(volatile uint32_t *)(uintptr_t)0xE0001000UL)
#define DWT_CYCCNT (*(volatile uint32_t *)(uintptr_t)0xE0001004UL)
#define DEMCR_TRCENA       (1UL << 24)
#define DWT_CYCCNTENA      (1UL << 0)

static bool g_available;

void hr_benchmark_clock_init(void)
{
    DEMCR |= DEMCR_TRCENA;
    DWT_CYCCNT = 0U;
    DWT_CTRL |= DWT_CYCCNTENA;
    g_available = (DWT_CTRL & DWT_CYCCNTENA) != 0U;
}

uint32_t hr_benchmark_now(void) { return DWT_CYCCNT; }
uint32_t hr_benchmark_elapsed(uint32_t start, uint32_t end) { return end - start; }
bool hr_benchmark_clock_available(void) { return g_available; }

uint32_t hr_benchmark_timestamp_overhead(void)
{
    uint32_t best = UINT32_MAX;
    for (uint32_t i = 0U; i < 32U; ++i)
    {
        const uint32_t start = hr_benchmark_now();
        const uint32_t end = hr_benchmark_now();
        const uint32_t elapsed = end - start;
        if (elapsed < best) { best = elapsed; }
    }
    return best;
}
