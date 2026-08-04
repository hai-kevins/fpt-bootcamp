#ifndef MODEL_H
#define MODEL_H
#include <stdbool.h>

typedef struct
{
    const char *name;
    unsigned base,
    effective;
    bool ready,
    blocked;
} task_t;

typedef struct
{
    task_t *owner;
    task_t *waiter;
} mutex_t;

void mutex_init(mutex_t *m);
bool lock_with_pi(mutex_t *m, task_t *t);
bool unlock_with_pi(mutex_t *m, task_t *t);
void waiter_timeout(mutex_t *m, task_t *t);
#endif
