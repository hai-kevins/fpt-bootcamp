#ifndef MODEL_H
#define MODEL_H
#include <stdbool.h>
#include <stdint.h>
typedef struct{uint32_t next;uint32_t period;uint32_t misses;}periodic_t;void periodic_init(periodic_t*p,uint32_t now,uint32_t period);uint32_t delay_until(periodic_t*p,uint32_t now);uint32_t relative_next(uint32_t now,uint32_t period);bool tick_reached(uint32_t now,uint32_t deadline);
#endif
