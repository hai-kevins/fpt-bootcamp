#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
static bool active, pressed;
static uint32_t deadline;
static void edge(uint32_t now)
{
    active = true;
    deadline = now + 20U;
}

static void tick(uint32_t now, bool pin_low)
{
    if (active && (int32_t)(now - deadline) >= 0)
    {
        active = false;
        if (pin_low)
        {
            pressed = true;
        }
    }
}

int main(void)
{
    edge(100U);
    tick(119U, true);
    assert(!pressed);
    tick(120U, true);
    assert(pressed);
    assert(!active);
    puts("one-shot debounce PASS");
    return 0;
}
