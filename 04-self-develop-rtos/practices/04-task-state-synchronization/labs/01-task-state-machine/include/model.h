#ifndef MODEL_H
#define MODEL_H
#include <stdbool.h>

typedef enum
{
    ST_UNUSED = 0,
    ST_CREATED,
    ST_READY,
    ST_RUNNING,
    ST_BLOCKED,
    ST_SUSPENDED
} state_t;

bool transition_allowed(state_t from, state_t to);
bool transition_apply(state_t *state, state_t to);
const char *state_name(state_t state);
#endif
