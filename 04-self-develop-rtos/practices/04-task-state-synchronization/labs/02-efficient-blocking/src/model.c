#include "model.h"

bool block_task(task_t *t)
{
    if (!t || t->state != RUNNING || !t->in_ready)
    {
        return false;
    }
    t->in_ready = false;
    t->state = BLOCKED;
    return true;
}

bool wake_task(task_t *t)
{
    if (!t || t->state != BLOCKED || t->in_ready)
    {
        return false;
    }
    t->state = READY;
    t->in_ready = true;
    return true;
}

bool suspend_task(task_t *t)
{
    if (!t || t->state == SUSPENDED)
    {
        return false;
    }
    t->in_ready = false;
    t->state = SUSPENDED;
    return true;
}

bool resume_task(task_t *t)
{
    if (!t || t->state != SUSPENDED)
    {
        return false;
    }
    t->state = READY;
    t->in_ready = true;
    return true;
}

bool validate_task(const task_t *t)
{
    if (!t)
    {
        return false;
    }
    if ((t->state == READY || t->state == RUNNING) != t->in_ready)
    {
        return false;
    }
    if ((t->state == BLOCKED || t->state == SUSPENDED) && t->in_ready)
    {
        return false;
    }
    return true;
}
