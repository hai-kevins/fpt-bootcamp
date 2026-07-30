#include "scheduler.h"

#include <limits.h>
#include <string.h>

void ed_scheduler_init(ed_scheduler_t *scheduler,
                       ed_event_pool_t *pool)
{
    if (scheduler != NULL)
    {
        (void)memset(scheduler, 0, sizeof(*scheduler));
        scheduler->pool = pool;
    }
}

bool ed_scheduler_register(ed_scheduler_t *scheduler,
                           ed_active_object_t *object)
{
    if ((scheduler == NULL) || (object == NULL) ||
        (scheduler->count >= ED_SCHEDULER_MAX_OBJECTS))
    {
        return false;
    }

    for (size_t i = 0U; i < scheduler->count; i++)
    {
        if (scheduler->objects[i]->id == object->id)
        {
            return false;
        }
    }

    scheduler->objects[scheduler->count] = object;
    scheduler->count++;
    return true;
}

bool ed_scheduler_run_once(ed_scheduler_t *scheduler)
{
    size_t selected = SIZE_MAX;
    uint8_t best_priority = 0U;

    if (scheduler == NULL)
    {
        return false;
    }

    for (size_t offset = 0U; offset < scheduler->count; offset++)
    {
        const size_t index = (scheduler->last_index + 1U + offset) %
                             scheduler->count;
        ed_active_object_t *object = scheduler->objects[index];

        if (ed_active_object_ready(object) &&
            ((selected == SIZE_MAX) ||
             (object->priority > best_priority)))
        {
            selected = index;
            best_priority = object->priority;
        }
    }

    if (selected == SIZE_MAX)
    {
        scheduler->idle_count++;
        return false;
    }

    ed_active_object_t *object = scheduler->objects[selected];
    ed_event_t *event = ed_mailbox_get(&object->mailbox);

    if (event == NULL)
    {
        return false;
    }

    scheduler->last_index = selected;
    object->handler(object, event);
    object->dispatch_count++;
    scheduler->dispatch_count++;

    if (scheduler->pool != NULL)
    {
        (void)ed_event_pool_release(scheduler->pool, event);
    }

    return true;
}

size_t ed_scheduler_run_until_idle(ed_scheduler_t *scheduler,
                                   size_t budget)
{
    size_t count = 0U;

    while ((count < budget) && ed_scheduler_run_once(scheduler))
    {
        count++;
    }

    return count;
}
