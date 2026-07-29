#include <stdint.h>
#include <stdio.h>

int main(void)
{
    const uint32_t button_arrival_ms = 300UL;
    const uint32_t delay_end_ms = 1000UL;
    const uint32_t latency_ms =
        delay_end_ms - button_arrival_ms;

    printf("LED ON at 0 ms\n");
    printf("CPU enters blocking delay until 1000 ms\n");
    printf("BUTTON event arrives at %lu ms\n",
           (unsigned long)button_arrival_ms);
    printf("BUTTON handled at %lu ms\n",
           (unsigned long)delay_end_ms);
    printf("Response latency = %lu ms\n",
           (unsigned long)latency_ms);

    return (latency_ms == 700UL) ? 0 : 1;
}
