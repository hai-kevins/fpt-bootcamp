#include "model.h"

void sem_init(sem_t *s, unsigned i, unsigned m)
{
    s->count = i;
    s->max = m;
    s->n = 0;
    s->seq = 0;
}

bool sem_take(sem_t *s, task_t *t, bool wait)
{
    if (!s || !t)
    {
        return false;
    }
    if (s->count)
    {
        --s->count;
        return true;
    }
    if (!wait || t->blocked || s->n >= 16)
    {
        return false;
    }
    t->blocked = true;
    t->seq = s->seq++;
    size_t i = 0;
    while (i < s->n && (s->waiters[i]->prio < t->prio || (s->waiters[i]->prio == t->prio && s->waiters[i]->seq < t->seq)))
    {
        ++i;
    }
    for (size_t j = s->n; j > i; --j)
    {
        s->waiters[j] = s->waiters[j - 1];
    }
    s->waiters[i] = t;
    ++s->n;
    return false;
}

bool sem_give(sem_t *s, task_t * *w)
{
    if (!s)
    {
        return false;
    }
    if (s->n)
    {
        task_t *t = s->waiters[0];
        for (size_t i = 1; i < s->n; ++i)
        {
            s->waiters[i - 1] = s->waiters[i];
        }
        --s->n;
        t->blocked = false;
        if (w)
        {
            *w = t;
        }
        return true;
    }
    if (s->count >= s->max)
    {
        return false;
    }
    ++s->count;
    if (w)
    {
        *w = 0;
    }
    return true;
}

bool sem_validate(const sem_t *s)
{
    if (!s || !s->max || s->count > s->max || s->n > 16)
    {
        return false;
    }
    for (size_t i = 0; i < s->n; ++i)
    {
        if (!s->waiters[i] || !s->waiters[i]->blocked)
        {
            return false;
        }
        if (i && s->waiters[i - 1]->prio > s->waiters[i]->prio)
        {
            return false;
        }
    }
    return true;
}
