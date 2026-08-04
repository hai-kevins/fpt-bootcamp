#include <stdint.h>
#include <stdio.h>

int main(void)
{
    const uint32_t polling_period_ms = 100UL;
    const uint32_t event_arrival_ms = 250UL;
    const uint32_t next_poll_ms = 300UL;
    printf("architecture=superloop\n");
    printf("global_flags=4\n");
    printf("main_branches=5\n");
    printf("event_latency_ms=%lu\n", (unsigned long)(next_poll_ms - event_arrival_ms));
    printf("blocking_risk=high\n");
    (void) polling_period_ms;
    return 0;
}
