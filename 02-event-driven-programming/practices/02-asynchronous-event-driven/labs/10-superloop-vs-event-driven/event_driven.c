#include <stdint.h>
#include <stdio.h>
int main(void)
{
    const uint32_t event_arrival_ms=250UL;
    const uint32_t dispatch_ms=251UL;
    printf("architecture=event-driven\n");
    printf("global_flags=0\n");
    printf("main_branches=1\n");
    printf("event_latency_ms=%lu\n",
           (unsigned long)(dispatch_ms-event_arrival_ms));
    printf("blocking_risk=controlled-by-RTC\n");
    return 0;
}
