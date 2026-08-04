#ifndef STACK_MONITOR_H
#define STACK_MONITOR_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void stack_monitor_fill(uint8_t *memory, size_t size, uint8_t pattern);
size_t stack_monitor_high_water(const uint8_t *memory, size_t size, uint8_t pattern, bool grows_down);

#endif
