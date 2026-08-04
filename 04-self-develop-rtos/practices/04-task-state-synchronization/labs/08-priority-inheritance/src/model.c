#include "model.h"

void mutex_init(mutex_t *m)
{
    m->owner = 0;
    m->waiter = 0;
}

bool lock_with_pi(mutex_t *m, task_t *t)
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
    if (m->owner == t)
    {
        return false;
    }
    m->waiter = t;
    t->blocked = true;
    t->ready = false;
    if (t->effective < m->owner->effective)
    {
        m->owner->effective = t->effective;
    }
    return false;
}

bool unlock_with_pi(mutex_t *m, task_t *t)
{
    if (!m || m->owner != t)
    {
        return false;
    }
    t->effective = t->base;
    if (m->waiter)
    {
        m->waiter->blocked = false;
        m->waiter->ready = true;
        m->owner = m->waiter;
        m->waiter = 0;
    }
    else
    {
        m->owner = 0;
    }
    return true;
}

void waiter_timeout(mutex_t *m, task_t *t)
{
    if (m && m->waiter == t)
    {
        m->waiter = 0;
        t->blocked = false;
        t->ready = true;
        if (m->owner)
        {
            m->owner->effective = m->owner->base;
        }
    }
}
