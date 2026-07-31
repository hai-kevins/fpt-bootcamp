#ifndef FAKE_TIME_H
#define FAKE_TIME_H

#include <stdint.h>

typedef struct
{
    uint32_t now_ms;
} fake_time_t;

void fake_time_reset(fake_time_t *time_source);
uint32_t fake_time_now(const fake_time_t *time_source);
void fake_time_advance(fake_time_t *time_source, uint32_t delta_ms);

#endif
