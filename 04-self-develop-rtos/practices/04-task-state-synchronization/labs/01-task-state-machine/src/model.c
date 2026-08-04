#include "model.h"

bool transition_allowed(state_t from, state_t to)
{
    switch (from)
    {
        case ST_CREATED : return to == ST_READY;
        case ST_READY : return to == ST_RUNNING || to == ST_SUSPENDED;
        case ST_RUNNING : return to == ST_READY || to == ST_BLOCKED || to == ST_SUSPENDED;
        case ST_BLOCKED : return to == ST_READY || to == ST_SUSPENDED;
        case ST_SUSPENDED : return to == ST_READY;
        default:
        return false;
    }
}

bool transition_apply(state_t *s, state_t to)
{
    if (s == 0 || !transition_allowed (*s, to))
    {
        return false;
    }
    *s = to;
    return true;
}

const char *state_name(state_t s)
{
    static const char *n[] =
    {
        "UNUSED",
        "CREATED",
        "READY",
        "RUNNING",
        "BLOCKED",
        "SUSPENDED"
    };
    return (s >= ST_UNUSED && s <= ST_SUSPENDED) ? n[s] : "UNKNOWN";
}
