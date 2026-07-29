#ifndef LAB04_SOFTWARE_TIMER_H
#define LAB04_SOFTWARE_TIMER_H
#include <stdbool.h>
#include <stdint.h>
void software_timer_init(void);
void software_timer_start(uint32_t now_ms, uint32_t delay_ms);
bool software_timer_expired(uint32_t now_ms);
#endif
