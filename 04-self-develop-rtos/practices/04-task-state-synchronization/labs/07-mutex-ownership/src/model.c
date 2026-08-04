#include "model.h"

void mutex_init(mutex_t *m)
{
    m->owner = 0;
    m->n = 0;
}

bool mutex_lock(mutex_t *m, task_t *t, bool wait)
{
    if (!m || !t)
    {
        return false;
    }
    if (!m->owner)
    {
        m->owner = t;
        return true;
    }
    if (m->owner == t || !wait || t->blocked || m->n >= 8)
    {
        return false;
    }
    t->blocked = true;
    size_t i = 0;
    while (i < m->n && m->waiters[i]->prio <= t->prio)
    {
        ++i;
    }
    for (size_t j = m->n; j > i; --j)
    {
        m->waiters[j] = m->waiters[j - 1];
    }
    m->waiters[i] = t;
    ++m->n;
    return false;
}

bool mutex_unlock(mutex_t *m, task_t *t)
{
    if (!m || m->owner != t)
    {
        return false;
    }
    if (m->n)
    {
        task_t *w = m->waiters[0];
        for (size_t i = 1; i < m->n; ++i)
        {
            m->waiters[i - 1] = m->waiters[i];
        }
        --m->n;
        w->blocked = false;
        m->owner = w;
    }
    else
    {
        m->owner = 0;
    }
    return true;
}

bool mutex_validate(const mutex_t *m)
{
    if (!m || m->n > 8)
    {
        return false;
    }
    for (size_t i = 0; i < m->n; ++i)
        if (!m->waiters[i] || !m->waiters[i]->blocked || m->waiters[i] == m->owner)
        {
            return false;
        }
    return true;
}
