#include "led_sm.h"
#include <stdbool.h>
#include <stdio.h>
int main(void)
{
    led_sm_t sm;
    bool pass = true;
    led_sm_init(&sm);
    led_sm_dispatch(&sm, LED_EVENT_ON, 0UL);
    pass = pass && (sm.state == LED_ON) && sm.output_on;
    led_sm_dispatch(&sm, LED_EVENT_BLINK_START, 100UL);
    pass = pass && (sm.state == LED_BLINKING) &&
           (sm.period_ms == 100UL);
    led_sm_dispatch(&sm, LED_EVENT_TIMEOUT, 0UL);
    pass = pass && !sm.output_on;
    led_sm_dispatch(&sm, LED_EVENT_BLINK_STOP, 0UL);
    pass = pass && (sm.state == LED_OFF) && !sm.output_on;
    printf("[%s] led-state-machine\n",
           pass ? "PASS" : "FAIL");
    return pass ? 0 : 1;
}
