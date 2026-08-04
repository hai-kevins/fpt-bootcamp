#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
enum
{
    LED_ON_SIG = 1,
    LED_OFF_SIG,
    LED_TOGGLE_SIG
};
static bool led;
static void handle(uint16_t sig)
{
    if (sig == LED_ON_SIG)
    {
        led = true;
    }
        else if (sig == LED_OFF_SIG)
        {
            led = false;
        }
            else if (sig == LED_TOGGLE_SIG)
            {
                led = !led;
            }
}

int main(void)
{
    handle(LED_ON_SIG);
    assert(led);
    handle(LED_TOGGLE_SIG);
    assert(!led);
    handle(LED_OFF_SIG);
    assert(!led);
    puts("pure message PASS");
    return 0;
}
