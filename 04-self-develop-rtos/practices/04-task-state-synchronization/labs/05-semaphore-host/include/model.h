#ifndef MODEL_H
#define MODEL_H
#include <stdbool.h>
#include <stddef.h>
typedef struct{const char*name;unsigned prio;bool blocked;unsigned seq;}task_t;typedef struct{unsigned count,max;task_t*waiters[16];size_t n;unsigned seq;}sem_t;void sem_init(sem_t*s,unsigned initial,unsigned max);bool sem_take(sem_t*s,task_t*t,bool wait);bool sem_give(sem_t*s,task_t**woken);bool sem_validate(const sem_t*s);
#endif
