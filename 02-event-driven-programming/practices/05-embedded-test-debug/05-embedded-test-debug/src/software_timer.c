#include "software_timer.h"

#include <string.h>

void software_timer_init(
    software_timer_service_t *service
)
{
    if (service != NULL)
    {
        (void)memset(service, 0, sizeof(*service));
    }
}

bool software_timer_start(
    software_timer_service_t *service,
    size_t timer_id,
    uint32_t now_ms,
    uint32_t delay_ms,
    bool periodic,
    const event_t *event
)
{
    if ((service == NULL) ||
        (event == NULL) ||
        (timer_id >= SOFTWARE_TIMER_MAX_COUNT) ||
        (delay_ms == 0U))
    {
        return false;
    }

    software_timer_entry_t *entry =
        &service->entries[timer_id];

    entry->active = true;
    entry->periodic = periodic;
    entry->deadline_ms = now_ms + delay_ms;
    entry->period_ms = delay_ms;
    entry->event = *event;
    service->arm_count++;
    return true;
}

bool software_timer_cancel(
    software_timer_service_t *service,
    size_t timer_id
)
{
    if ((service == NULL) ||
        (timer_id >= SOFTWARE_TIMER_MAX_COUNT))
    {
        return false;
    }

    software_timer_entry_t *entry =
        &service->entries[timer_id];

    if (!entry->active)
    {
        return false;
    }

    entry->active = false;
    service->cancel_count++;
    return true;
}

void software_timer_process(
    software_timer_service_t *service,
    uint32_t now_ms,
    event_queue_t *queue
)
{
    if ((service == NULL) || (queue == NULL))
    {
        return;
    }

    for (size_t i = 0U; i < SOFTWARE_TIMER_MAX_COUNT; i++)
    {
        software_timer_entry_t *entry =
            &service->entries[i];

        if ((!entry->active) ||
            (now_ms < entry->deadline_ms))
        {
            continue;
        }

        entry->event.timestamp_ms = now_ms;

        if (event_queue_post(queue, &entry->event))
        {
            service->expiry_count++;
        }
        else
        {
            service->post_failures++;
        }

        if (entry->periodic)
        {
            do
            {
                entry->deadline_ms += entry->period_ms;
            } while (entry->deadline_ms <= now_ms);
        }
        else
        {
            entry->active = false;
        }
    }
}
