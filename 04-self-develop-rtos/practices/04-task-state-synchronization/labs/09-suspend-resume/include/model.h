#ifndef MODEL_H
#define MODEL_H

#include <stdbool.h>

typedef enum
{
    READY = 0,
    RUNNING,
    BLOCKED,
    SUSPENDED
} state_t;

typedef struct
{
    state_t state;
    bool ready_link,
    wait_link,
    timeout_link;
    unsigned result;
} task_t;

bool suspend_task(task_t *t);
bool resume_task(task_t *t);
bool timeout_wake(task_t *t);
bool validate_task(const task_t *t);

#endif
