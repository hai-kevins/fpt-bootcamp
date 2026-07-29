#include "dispatcher.h"

#include "event_queue.h"
#include "event_trace.h"
#include "platform.h"

#include <stddef.h>

static event_queue_t g_queue;
static event_handler_t g_handlers[EVENT_DESTINATION_COUNT];
static uint32_t g_invalid_destination_count;

void dispatcher_init(void)
{
    event_queue_init(&g_queue);

    for (uint32_t i = 0UL;
         i < (uint32_t)EVENT_DESTINATION_COUNT;
         i++)
    {
        g_handlers[i] = NULL;
    }

    g_invalid_destination_count = 0UL;
}

bool dispatcher_register(
    event_destination_t destination,
    event_handler_t handler
)
{
    if (((uint32_t)destination >=
         (uint32_t)EVENT_DESTINATION_COUNT) ||
        (destination == EVENT_DESTINATION_NONE) ||
        (handler == NULL))
    {
        return false;
    }

    g_handlers[destination] = handler;
    return true;
}

bool dispatcher_post_event(const event_t *event)
{
    uint32_t key;
    bool posted;

    if (event == NULL)
    {
        return false;
    }

    key = platform_critical_enter();
    posted = event_queue_post(&g_queue, event);
    event_trace_record(
        posted ? EVENT_TRACE_POST : EVENT_TRACE_DROP,
        event,
        0U
    );
    platform_critical_exit(key);

    return posted;
}

bool dispatcher_post(
    event_source_t source,
    event_destination_t destination,
    signal_t signal,
    uint32_t parameter,
    uint32_t timestamp_ms
)
{
    const event_t event =
    {
        .timestamp_ms = timestamp_ms,
        .parameter = parameter,
        .signal = (uint16_t)signal,
        .source = (uint8_t)source,
        .destination = (uint8_t)destination
    };

    return dispatcher_post_event(&event);
}

bool dispatcher_try_get(event_t *event)
{
    uint32_t key;
    bool result;

    key = platform_critical_enter();
    result = event_queue_get(&g_queue, event);
    platform_critical_exit(key);

    return result;
}

bool dispatcher_dispatch_once(void)
{
    event_t event;
    event_handler_t handler;

    if (!dispatcher_try_get(&event))
    {
        return false;
    }

    {
        const uint32_t key = platform_critical_enter();
        event_trace_record(EVENT_TRACE_DISPATCH, &event, 0U);
        platform_critical_exit(key);
    }

    if ((event.destination == 0U) ||
        (event.destination >=
         (uint8_t)EVENT_DESTINATION_COUNT))
    {
        const uint32_t key = platform_critical_enter();
        g_invalid_destination_count++;
        event_trace_record(EVENT_TRACE_DROP, &event, 0U);
        platform_critical_exit(key);
        return true;
    }

    handler = g_handlers[event.destination];

    if (handler == NULL)
    {
        const uint32_t key = platform_critical_enter();
        g_invalid_destination_count++;
        event_trace_record(EVENT_TRACE_DROP, &event, 0U);
        platform_critical_exit(key);
        return true;
    }

    handler(&event);

    {
        const uint32_t key = platform_critical_enter();
        event_trace_record(EVENT_TRACE_COMPLETE, &event, 0U);
        platform_critical_exit(key);
    }

    return true;
}

void dispatcher_run_until_empty(void)
{
    while (dispatcher_dispatch_once())
    {
    }
}

uint16_t dispatcher_pending(void)
{
    uint32_t key;
    uint16_t count;

    key = platform_critical_enter();
    count = event_queue_count(&g_queue);
    platform_critical_exit(key);

    return count;
}

dispatcher_stats_t dispatcher_get_stats(void)
{
    dispatcher_stats_t stats;
    uint32_t key;

    key = platform_critical_enter();

    stats.posted = g_queue.post_count;
    stats.dispatched = g_queue.get_count;
    stats.dropped = g_queue.overflow_count;
    stats.invalid_destination = g_invalid_destination_count;
    stats.pending = g_queue.count;
    stats.high_water_mark = g_queue.high_water_mark;

    platform_critical_exit(key);

    return stats;
}
