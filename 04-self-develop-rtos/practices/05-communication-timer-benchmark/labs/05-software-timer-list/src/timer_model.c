#include "timer_model.h"
#include <stdint.h>

static bool before(uint32_t a, uint32_t b)
{
    return (int32_t)(a - b) < 0;
}

static bool reached(uint32_t n, uint32_t d)
{
    return (int32_t)(n - d) >= 0;
}

void timer_list_init(timer_list_t *l)
{
    l->head = 0;
    l->count = 0;
}

static void insert(timer_list_t *l, timer_model_t *t)
{
    timer_model_t * *p = &l->head;
    while (*p && !before(t->expiry, (*p)->expiry))
    {
        p = &(*p)->next;
    }
    t->next = *p;
    *p = t;
    l->count++;
}

bool timer_stop(timer_list_t *l, timer_model_t *t)
{
    timer_model_t * *p = &l->head;
    while (*p && *p != t)
    {
        p = &(*p)->next;
    }
    if (*p)
    {
        *p = t->next;
        l->count--;
    }
    t->next = 0;
    t->active = false;
    return true;
}

bool timer_start(timer_list_t *l, timer_model_t *t, uint32_t now, uint32_t delay, uint32_t period)
{
    if (!l || !t || !delay)
    {
        return false;
    }
    if (t->active)
    {
        timer_stop(l, t);
    }
    t->expiry = now + delay;
    t->period = period;
    t->active = true;
    insert(l, t);
    return true;
}

timer_model_t *timer_take_due(timer_list_t *l, uint32_t now)
{
    timer_model_t *t = l->head;
    if (!t || !reached(now, t->expiry))
    {
        return 0;
    }
    l->head = t->next;
    l->count--;
    t->next = 0;
    t->active = false;
    return t;
}

void timer_complete(timer_list_t *l, timer_model_t *t, uint32_t now)
{
    t->count++;
    if (t->period)
    {
        uint32_t next = t->expiry + t->period;
        while (reached(now, next))
        {
            next += t->period;
        }
        t->expiry = next;
        t->active = true;
        insert(l, t);
    }
}

bool timer_validate(const timer_list_t *l)
{
    size_t n = 0;
    const timer_model_t *p = l->head, *prev = 0;
    while (p)
    {
        if (prev && before(p->expiry, prev->expiry))
        {
            return false;
        }
        n++;
        if (n > 100)
        {
            return false;
        }
        prev = p;
        p = p->next;
    }
    return n == l->count;
}
