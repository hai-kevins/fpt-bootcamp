#include "stack_monitor.h"
#include <stdint.h>
#include <stdio.h>

int main(void)
{
    uint8_t stack[256];
    size_t index;

    stack_monitor_fill(stack, sizeof(stack), 0xA5U);

    for (index = 0U; index < 96U; index++)
    {
        stack[sizeof(stack) - 1U - index] = (uint8_t) index;
    }

    printf("Lab 03 - Stack high-water mark\n");
    printf("Stack capacity : %zu bytes\n", sizeof(stack));
    printf("Stack used     : %zu bytes\n", stack_monitor_high_water(stack, sizeof(stack), 0xA5U, true));
    return 0;
}
