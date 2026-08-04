#include "software_timer.h"

#include "dispatcher.h"

typedef struct
{
    bool active;
    bool periodic;
    event_destination_t destination;
    signal_t signal;
    uint32_t parameter;
    uint32_t deadline_ms;
    uint32_t period_ms;
} timer_slot_t;

static timer_slot_t g_timers[SOFTWARE_TIMER_COUNT];

static bool time_reached(uint32_t now_ms, uint32_t deadline_ms)
{
    return (int32_t)(now_ms - deadline_ms) >= 0;
}

void software_timer_init(void)
{
    for (uint32_t i = 0UL; i < (uint32_t) SOFTWARE_TIMER_COUNT; i++)
    {
        g_timers[i].active = false;
        g_timers[i].periodic = false;
        g_timers[i].destination = EVENT_DESTINATION_NONE;
        g_timers[i].signal = SIGNAL_NONE;
        g_timers[i].parameter = 0UL;
        g_timers[i].deadline_ms = 0UL;
        g_timers[i].period_ms = 0UL;
    }
}

bool software_timer_start(software_timer_id_t id, event_destination_t destination, signal_t signal, uint32_t parameter,
    uint32_t delay_ms, bool periodic, uint32_t now_ms)
{
    timer_slot_t *timer;

    if (((uint32_t) id >= (uint32_t) SOFTWARE_TIMER_COUNT) || (destination == EVENT_DESTINATION_NONE) || (signal == SIGNAL_NONE)
        || (delay_ms == 0UL))
    {
        return false;
    }

    timer = &g_timers[id];
    timer->active = true;
    timer->periodic = periodic;
    timer->destination = destination;
    timer->signal = signal;
    timer->parameter = parameter;
    timer->deadline_ms = now_ms + delay_ms;
    timer->period_ms = delay_ms;

    return true;
}

bool software_timer_cancel(software_timer_id_t id)
{
    if ((uint32_t) id >= (uint32_t) SOFTWARE_TIMER_COUNT)
    {
        return false;
    }

    g_timers[id].active = false;
    return true;
}

bool software_timer_is_active(software_timer_id_t id)
{
    if ((uint32_t) id >= (uint32_t) SOFTWARE_TIMER_COUNT)
    {
        return false;
    }

    return g_timers[id].active;
}

void software_timer_process(uint32_t now_ms)
{
    for (uint32_t i = 0UL; i < (uint32_t) SOFTWARE_TIMER_COUNT; i++)
    {
        timer_slot_t *timer = &g_timers[i];

        if (!timer->active || !time_reached(now_ms, timer->deadline_ms))
        {
            continue;
        }

        (void) dispatcher_post(EVENT_SOURCE_TIMER, timer->destination, timer->signal, timer->parameter, now_ms);

        if (timer->periodic)
        {
            do
            {
                timer->deadline_ms += timer->period_ms;
            }
            while (time_reached(now_ms, timer->deadline_ms));
        }
        else
        {
            timer->active = false;
        }
    }
}
