#include "timer.h"

#include "ak_config.h"
#include "ak_port.h"
#include "event_record.h"
#include "message.h"
#include "task.h"

#include "runtime.h"

typedef struct
{
    bool active;
    bool periodic;
    uint8_t source;
    uint8_t destination;
    uint16_t signal;
    uint32_t deadline;
    uint32_t period;
} timer_slot_t;

static timer_slot_t g_timers[AK_TIMER_CAPACITY];
static ak_timer_stats_t g_stats;

static bool deadline_reached(uint32_t now, uint32_t deadline)
{
    return (int32_t)(now - deadline) >= 0;
}

static ak_timer_id_t timer_start(
    uint8_t source,
    uint8_t destination,
    uint16_t signal,
    uint32_t period_ms,
    bool periodic
)
{
    if (period_ms == 0U)
    {
        return AK_TIMER_ID_INVALID;
    }

    for (uint8_t i = 0U; i < AK_TIMER_CAPACITY; i++)
    {
        if (!g_timers[i].active)
        {
            g_timers[i].active = true;
            g_timers[i].periodic = periodic;
            g_timers[i].source = source;
            g_timers[i].destination = destination;
            g_timers[i].signal = signal;
            g_timers[i].period = period_ms;
            g_timers[i].deadline = ak_port_time_now_ms() + period_ms;
            g_stats.active_count++;
            if (g_stats.active_count > g_stats.active_max)
            {
                g_stats.active_max = g_stats.active_count;
            }
            g_stats.starts++;

            ak_event_record_t record = {
                .timestamp = ak_port_time_now_ms(),
                .signal = signal,
                .value = i,
                .type = AK_RECORD_TIMER_START,
                .source = source,
                .destination = destination,
                .state = periodic ? 1U : 0U
            };
            ak_event_record_write(&record);
            return i;
        }
    }

    return AK_TIMER_ID_INVALID;
}

void ak_timer_system_init(void)
{
    (void)memset(g_timers, 0, sizeof(g_timers));
    (void)memset(&g_stats, 0, sizeof(g_stats));
}

ak_timer_id_t ak_timer_start_one_shot(
    uint8_t source,
    uint8_t destination,
    uint16_t signal,
    uint32_t delay_ms
)
{
    return timer_start(source, destination, signal, delay_ms, false);
}

ak_timer_id_t ak_timer_start_periodic(
    uint8_t source,
    uint8_t destination,
    uint16_t signal,
    uint32_t period_ms
)
{
    return timer_start(source, destination, signal, period_ms, true);
}

bool ak_timer_restart(ak_timer_id_t timer_id, uint32_t period_ms)
{
    if ((timer_id >= AK_TIMER_CAPACITY) || !g_timers[timer_id].active ||
        (period_ms == 0U))
    {
        return false;
    }

    g_timers[timer_id].period = period_ms;
    g_timers[timer_id].deadline = ak_port_time_now_ms() + period_ms;
    return true;
}

bool ak_timer_cancel(ak_timer_id_t timer_id)
{
    if ((timer_id >= AK_TIMER_CAPACITY) || !g_timers[timer_id].active)
    {
        return false;
    }

    g_timers[timer_id].active = false;
    if (g_stats.active_count > 0U)
    {
        g_stats.active_count--;
    }
    return true;
}

void ak_timer_process(uint32_t now_ms)
{
    for (uint8_t i = 0U; i < AK_TIMER_CAPACITY; i++)
    {
        if (g_timers[i].active && deadline_reached(now_ms, g_timers[i].deadline))
        {
            ak_message_t *message;
            g_stats.expirations++;

            ak_event_record_t record = {
                .timestamp = now_ms,
                .signal = g_timers[i].signal,
                .value = i,
                .type = AK_RECORD_TIMER_EXPIRE,
                .source = g_timers[i].source,
                .destination = g_timers[i].destination,
                .state = g_timers[i].periodic ? 1U : 0U
            };
            ak_event_record_write(&record);

            message = ak_message_create_pure(
                g_timers[i].source,
                g_timers[i].destination,
                g_timers[i].signal
            );

            if ((message == 0) || !ak_task_post(message))
            {
                if (message != 0)
                {
                    ak_message_release(message);
                }
                g_stats.post_failures++;
            }

            if (g_timers[i].periodic)
            {
                do
                {
                    g_timers[i].deadline += g_timers[i].period;
                }
                while (deadline_reached(now_ms, g_timers[i].deadline));
            }
            else
            {
                g_timers[i].active = false;
                if (g_stats.active_count > 0U)
                {
                    g_stats.active_count--;
                }
            }
        }
    }
}

ak_timer_stats_t ak_timer_stats(void)
{
    return g_stats;
}
