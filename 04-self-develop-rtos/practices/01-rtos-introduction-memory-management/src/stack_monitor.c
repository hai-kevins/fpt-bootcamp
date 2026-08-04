#include "stack_monitor.h"

void stack_monitor_fill(uint8_t *memory, size_t size, uint8_t pattern)
{
    size_t index;
    if (memory == (uint8_t *)0)
    {
        return;
    }
    for (index = 0U; index < size; index++)
    {
        memory[index] = pattern;
    }
}

size_t stack_monitor_high_water(const uint8_t *memory, size_t size, uint8_t pattern)
{
    size_t unused = 0U;
    if (memory == (const uint8_t *)0)
    {
        return 0U;
    }
    while ((unused < size) && (memory[unused] == pattern))
    {
        unused++;
    }
    return size - unused;
}
