#ifndef MODEL_H
#define MODEL_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
typedef struct{const char*name;uint32_t wake;bool linked;}task_t;typedef struct{task_t*items[16];size_t count;}delayed_t;
bool tick_reached(uint32_t now,uint32_t deadline);bool tick_before(uint32_t a,uint32_t b);void delayed_init(delayed_t*d);bool delayed_insert(delayed_t*d,task_t*t);task_t*delayed_take_due(delayed_t*d,uint32_t now);bool delayed_validate(const delayed_t*d);
#endif
