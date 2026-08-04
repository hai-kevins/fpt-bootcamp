#ifndef MODEL_H
#define MODEL_H

#include <stdbool.h>
#include <stddef.h>

typedef struct
{
    const char *name;
    unsigned prio;
    bool blocked;
} task_t;

typedef struct
{
    task_t *owner;
    task_t *waiters[8];
    size_t n;
} mutex_t;

void mutex_init(mutex_t *m);
bool mutex_lock(mutex_t *m, task_t *t, bool wait);
bool mutex_unlock(mutex_t *m, task_t *t);
bool mutex_validate(const mutex_t *m);

#endif
