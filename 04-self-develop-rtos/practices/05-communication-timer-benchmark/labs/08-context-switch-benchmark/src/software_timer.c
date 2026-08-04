#include "software_timer.h"
#include "critical_section.h"
#include "scheduler.h"
#include "timeout.h"

static hr_list_t g_active_timers;
static hr_semaphore_t *g_service_signal;

static bool insert_locked(hr_software_timer_t *timer)
{
    hr_list_node_t *node = hr_list_front(&g_active_timers);
    while (node != (hr_list_node_t *)0)
    {
        hr_software_timer_t *other = HR_CONTAINER_OF(node, hr_software_timer_t, node);
        if (hr_tick_before(timer->expiry_tick, other->expiry_tick))
        {
            return hr_list_insert_before(&g_active_timers, node, &timer->node);
        }
        node = node->next;
    }
    return hr_list_push_back(&g_active_timers, &timer->node);
}

void hr_software_timer_system_init(hr_semaphore_t *service_signal)
{
    hr_list_init(&g_active_timers);
    g_service_signal = service_signal;
}

bool hr_software_timer_init(hr_software_timer_t *timer, const char *name, hr_timer_callback_t callback, void *argument)
{
    if ((timer == (hr_software_timer_t *)0) || (name == (const char *)0) || (callback == (hr_timer_callback_t)0))
    {
        return false;
    }
    hr_list_node_init(&timer->node);
    timer->name = name;
    timer->expiry_tick = 0U;
    timer->period_ticks = 0U;
    timer->expiry_count = 0U;
    timer->missed_period_count = 0U;
    timer->callback = callback;
    timer->argument = argument;
    timer->state = HR_TIMER_INACTIVE;
    timer->periodic = false;
    return true;
}

bool hr_software_timer_start(hr_software_timer_t *timer, uint32_t delay_ticks, uint32_t period_ticks)
{
    bool ok;
    const hr_irq_state_t state = hr_critical_enter();
    if ((timer == (hr_software_timer_t *)0) || (delay_ticks == 0U) || (timer->state == HR_TIMER_CALLBACK_PENDING))
    {
        hr_critical_exit(state);
        return false;
    }
    if (timer->node.owner == &g_active_timers)
    {
        (void) hr_list_remove(&g_active_timers, &timer->node);
    }
    timer->expiry_tick = hr_scheduler_tick_now() + delay_ticks;
    timer->period_ticks = period_ticks;
    timer->periodic = period_ticks > 0U;
    timer->state = HR_TIMER_ACTIVE;
    ok = insert_locked(timer);
    if (!ok)
    {
        timer->state = HR_TIMER_INACTIVE;
    }
    hr_critical_exit(state);
    return ok;
}

bool hr_software_timer_stop(hr_software_timer_t *timer)
{
    const hr_irq_state_t state = hr_critical_enter();
    if ((timer == (hr_software_timer_t *)0) || (timer->state == HR_TIMER_CALLBACK_PENDING))
    {
        hr_critical_exit(state);
        return false;
    }
    if (timer->node.owner == &g_active_timers)
    {
        (void) hr_list_remove(&g_active_timers, &timer->node);
    }
    timer->state = HR_TIMER_INACTIVE;
    timer->periodic = false;
    timer->period_ticks = 0U;
    hr_critical_exit(state);
    return true;
}

bool hr_software_timer_restart(hr_software_timer_t *timer, uint32_t delay_ticks)
{
    const uint32_t period = (timer != (hr_software_timer_t *)0) ? timer->period_ticks : 0U;
    return hr_software_timer_start(timer, delay_ticks, period);
}

void hr_software_timer_on_tick_from_isr(uint32_t now)
{
    bool higher = false;
    const hr_irq_state_t state = hr_critical_enter();
    hr_list_node_t *node = hr_list_front(&g_active_timers);
    if (node != (hr_list_node_t *)0)
    {
        const hr_software_timer_t *timer = HR_CONTAINER_OF(node, hr_software_timer_t, node);
        if (hr_tick_reached(now, timer->expiry_tick) && (g_service_signal != (hr_semaphore_t *)0))
        {
            (void) hr_semaphore_give_from_isr(g_service_signal, &higher);
        }
    }
    hr_critical_exit(state);
    (void) higher;
}

hr_software_timer_t *hr_software_timer_take_due(uint32_t now)
{
    hr_software_timer_t *timer = (hr_software_timer_t *)0;
    const hr_irq_state_t state = hr_critical_enter();
    hr_list_node_t *node = hr_list_front(&g_active_timers);
    if (node != (hr_list_node_t *)0)
    {
        hr_software_timer_t *candidate = HR_CONTAINER_OF(node, hr_software_timer_t, node);
        if (hr_tick_reached(now, candidate->expiry_tick))
        {
            (void) hr_list_remove(&g_active_timers, &candidate->node);
            candidate->state = HR_TIMER_CALLBACK_PENDING;
            timer = candidate;
        }
    }
    hr_critical_exit(state);
    return timer;
}

void hr_software_timer_complete(hr_software_timer_t *timer, uint32_t now)
{
    const hr_irq_state_t state = hr_critical_enter();
    if ((timer == (hr_software_timer_t *)0) || (timer->state != HR_TIMER_CALLBACK_PENDING))
    {
        hr_critical_exit(state);
        return;
    }
    ++timer->expiry_count;
    if (timer->periodic && (timer->period_ticks > 0U))
    {
        uint32_t next = timer->expiry_tick + timer->period_ticks;
        while (hr_tick_reached(now, next))
        {
            next += timer->period_ticks;
            ++timer->missed_period_count;
        }
        timer->expiry_tick = next;
        timer->state = HR_TIMER_ACTIVE;
        if (!insert_locked(timer))
        {
            timer->state = HR_TIMER_INACTIVE;
        }
    }
    else
    {
        timer->state = HR_TIMER_INACTIVE;
    }
    hr_critical_exit(state);
}

size_t hr_software_timer_active_count(void)
{
    return hr_list_count(&g_active_timers);
}

const hr_list_t *hr_software_timer_active_list(void)
{
    return &g_active_timers;
}

bool hr_software_timer_validate(void)
{
    const hr_list_node_t *node;
    const hr_software_timer_t *previous = (const hr_software_timer_t *)0;
    if (!hr_list_validate(&g_active_timers))
    {
        return false;
    }
    node = g_active_timers.head;
    while (node != (const hr_list_node_t *)0)
    {
        const hr_software_timer_t *timer = HR_CONTAINER_OF(node, hr_software_timer_t, node);
        if ((timer->name == (const char *)0) || (timer->callback == (hr_timer_callback_t)0) || (timer->state != HR_TIMER_ACTIVE))
        {
            return false;
        }
        if ((previous != (const hr_software_timer_t *)0) && hr_tick_before(timer->expiry_tick, previous->expiry_tick))
        {
            return false;
        }
        previous = timer;
        node = node->next;
    }
    return true;
}
