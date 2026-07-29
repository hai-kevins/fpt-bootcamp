#include "led_sm.h"
#include <stdio.h>
int main(void)
{
    led_sm_t sm;
    led_sm_init(&sm);
    led_sm_dispatch(&sm, LED_EVENT_BLINK_START, 250UL);
    printf("state=%d output=%d period=%lu\n",
           (int)sm.state,
           sm.output_on ? 1 : 0,
           (unsigned long)sm.period_ms);
    led_sm_dispatch(&sm, LED_EVENT_TIMEOUT, 0UL);
    printf("after-timeout output=%d\n",
           sm.output_on ? 1 : 0);
    return 0;
}
