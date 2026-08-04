#include "scheduler.h"
#include "cortex_m3_port.h"
#include "critical_section.h"
#include "mutex.h"
#include "panic.h"
#include "ready_queue.h"
#include "task_registry.h"
#include "timeout.h"
#include "wait_list.h"

hr_task_t *g_current_task;
volatile uint32_t g_kernel_tick;
volatile uint32_t g_context_switch_count;

static bool priority_higher_than_current(const hr_task_t *task)
{
    return (task != (const hr_task_t *)0) && ((g_current_task == (hr_task_t *)0) || (task->effective_priority < g_current_task->effective_priority));
}

void hr_scheduler_init(void)
{
    hr_task_registry_init();
    hr_ready_init();
    hr_timeout_init();
    g_current_task = (hr_task_t *)0;
    g_kernel_tick = 0U;
    g_context_switch_count = 0U;
}

bool hr_scheduler_add_task(hr_task_t *task)
{
    const hr_irq_state_t state = hr_critical_enter();
    bool ok = false;
    if (hr_task_is_valid(task) && (task->state == HR_TASK_CREATED) && (task->effective_priority < HR_PRIORITY_COUNT))
    {
        ok = hr_task_registry_add(task) && hr_ready_insert(task);
    }
    hr_critical_exit(state);
    return ok;
}

void hr_scheduler_start(void)
{
    hr_port_init();
    g_current_task = hr_ready_select_highest();
    if (g_current_task == (hr_task_t *)0)
    {
        hr_panic(HR_PANIC_NO_READY_TASK, 0U);
    }
    g_current_task->state = HR_TASK_RUNNING;
    ++g_current_task->switch_count;
    hr_port_start_first_task();
}

void hr_scheduler_commit_switch(void)
{
    hr_task_t *old_task = g_current_task;
    hr_task_t *next_task = hr_ready_select_highest();
    if (next_task == (hr_task_t *)0)
    {
        hr_panic(HR_PANIC_NO_READY_TASK, hr_ready_bitmap());
    }
    if ((old_task != (hr_task_t *)0) && (old_task != next_task) && (old_task->state == HR_TASK_RUNNING))
    {
        old_task->state = HR_TASK_READY;
    }
    next_task->state = HR_TASK_RUNNING;
    g_current_task = next_task;
    if (old_task != next_task)
    {
        ++g_context_switch_count;
        ++next_task->switch_count;
    }
}

bool hr_scheduler_wake_task_locked(hr_task_t *task, hr_wait_result_t result)
{
    if (!hr_task_is_valid(task) || (task->state != HR_TASK_BLOCKED))
    {
        return false;
    }

    if (task->wait_node.owner != (const void *)0)
    {
        hr_list_t *list = (hr_list_t *) task->wait_node.owner;
        if (!hr_list_remove(list, &task->wait_node))
        {
            return false;
        }
    }
    if (task->timeout_node.owner != (const void *)0)
    {
        if (!hr_timeout_remove(task))
        {
            return false;
        }
    }

    task->waiting_object = (void *)0;
    task->waiting_list = (hr_list_t *)0;
    task->wake_tick = HR_WAIT_FOREVER;
    task->wait_result = result;
    task->wait_kind = HR_WAIT_KIND_NONE;
    task->state = HR_TASK_READY;
    if (!hr_ready_insert(task))
    {
        return false;
    }
    if (priority_higher_than_current(task))
    {
        hr_port_request_context_switch();
    }
    return true;
}

bool hr_scheduler_block_current_locked(void *object, hr_list_t *waiters, hr_wait_kind_t kind, uint32_t timeout_ticks)
{
    hr_task_t *task = g_current_task;
    if (!hr_task_is_valid(task) || (task->state != HR_TASK_RUNNING) || (task->name[0] == 'i' && task->name[1] == 'd'
        && task->name[2] == 'l' && task->name[3] == 'e'))
    {
        return false;
    }

    if (!hr_ready_remove(task))
    {
        return false;
    }
    task->waiting_object = object;
    task->waiting_list = waiters;
    task->wait_result = HR_WAIT_NONE;
    task->wait_kind = kind;
    task->wake_tick = HR_WAIT_FOREVER;
    task->state = HR_TASK_BLOCKED;

    if ((waiters != (hr_list_t *)0) && !hr_wait_list_insert(waiters, task))
    {
        task->state = HR_TASK_READY;
        task->waiting_object = (void *)0;
        task->waiting_list = (hr_list_t *)0;
        (void) hr_ready_insert(task);
        return false;
    }

    if ((timeout_ticks != HR_WAIT_FOREVER) && (timeout_ticks != HR_NO_WAIT))
    {
        task->wake_tick = g_kernel_tick + timeout_ticks;
        if (!hr_timeout_insert(task))
        {
            if (task->wait_node.owner != (const void *)0)
            {
                (void) hr_list_remove(waiters, &task->wait_node);
            }
            task->state = HR_TASK_READY;
            task->waiting_object = (void *)0;
            task->waiting_list = (hr_list_t *)0;
            task->wake_tick = HR_WAIT_FOREVER;
            (void) hr_ready_insert(task);
            return false;
        }
    }

    hr_port_request_context_switch();
    return true;
}

static void process_timeouts(void)
{
    hr_task_t *task;
    while ((task = hr_timeout_take_due(g_kernel_tick)) != (hr_task_t *)0)
    {
        const hr_wait_kind_t kind = task->wait_kind;
        void *object = task->waiting_object;
        if (task->wait_node.owner != (const void *)0)
        {
            hr_list_t *list = (hr_list_t *) task->wait_node.owner;
            (void) hr_list_remove(list, &task->wait_node);
        }
        task->waiting_object = (void *)0;
        task->waiting_list = (hr_list_t *)0;
        task->wake_tick = HR_WAIT_FOREVER;
        task->wait_result = (kind == HR_WAIT_KIND_DELAY) ? HR_WAIT_SUCCESS : HR_WAIT_TIMEOUT;
        task->wait_kind = HR_WAIT_KIND_NONE;
        task->state = HR_TASK_READY;
        if (!hr_ready_insert(task))
        {
            hr_panic(HR_PANIC_ASSERT, (uint32_t) __LINE__);
        }
        if (kind == HR_WAIT_KIND_MUTEX)
        {
            hr_mutex_waiter_removed_locked(object);
        }
        if (priority_higher_than_current(task))
        {
            hr_port_request_context_switch();
        }
    }
}

void hr_scheduler_on_tick(void)
{
    bool switch_required = false;
    const hr_irq_state_t irq_state = hr_critical_enter();
    ++g_kernel_tick;
    process_timeouts();

    if ((g_current_task != (hr_task_t *)0) && (g_current_task->state == HR_TASK_RUNNING))
    {
        ++g_current_task->runtime_ticks;
        if (g_current_task->time_slice_remaining > 0U)
        {
            --g_current_task->time_slice_remaining;
        }
        if (g_current_task->time_slice_remaining == 0U)
        {
            g_current_task->time_slice_remaining = HR_DEFAULT_TIME_SLICE_TICKS;
            if (hr_ready_count(g_current_task->effective_priority) > 1U)
            {
                g_current_task->state = HR_TASK_READY;
                if (!hr_ready_rotate(g_current_task->effective_priority))
                {
                    hr_panic(HR_PANIC_ASSERT, (uint32_t) __LINE__);
                }
                switch_required = true;
            }
        }
    }
    if (switch_required)
    {
        hr_port_request_context_switch();
    }
    hr_critical_exit(irq_state);
}

void hr_task_yield(void)
{
    const hr_irq_state_t irq_state = hr_critical_enter();
    if ((g_current_task != (hr_task_t *)0) && (hr_ready_count(g_current_task->effective_priority) > 1U))
    {
        g_current_task->state = HR_TASK_READY;
        (void) hr_ready_rotate(g_current_task->effective_priority);
        hr_port_request_context_switch();
    }
    hr_critical_exit(irq_state);
}

hr_wait_result_t hr_task_delay(uint32_t ticks)
{
    hr_task_t *task;
    const hr_irq_state_t irq_state = hr_critical_enter();
    if (ticks == 0U)
    {
        hr_critical_exit(irq_state);
        hr_task_yield();
        return HR_WAIT_SUCCESS;
    }
    task = g_current_task;
    if (!hr_scheduler_block_current_locked((void *)0, (hr_list_t *)0, HR_WAIT_KIND_DELAY, ticks))
    {
        hr_critical_exit(irq_state);
        return HR_WAIT_CANCELLED;
    }
    hr_critical_exit(irq_state);
    return task->wait_result;
}

hr_wait_result_t hr_task_delay_until(uint32_t *previous_wake, uint32_t period_ticks)
{
    uint32_t now;
    uint32_t next;
    if ((previous_wake == (uint32_t *)0) || (period_ticks == 0U))
    {
        return HR_WAIT_CANCELLED;
    }
    now = g_kernel_tick;
    next = *previous_wake + period_ticks;
    *previous_wake = next;
    if (hr_tick_reached(now, next))
    {
        return HR_WAIT_SUCCESS;
    }
    return hr_task_delay(next - now);
}

bool hr_scheduler_set_effective_priority_locked(hr_task_t *task, uint8_t priority)
{
    bool linked;
    if (!hr_task_is_valid(task) || (priority >= HR_PRIORITY_COUNT))
    {
        return false;
    }
    if (task->effective_priority == priority)
    {
        return true;
    }
    linked = task->ready_node.owner != (const void *)0;
    if (linked && !hr_ready_remove(task))
    {
        return false;
    }
    task->effective_priority = priority;
    if (linked && !hr_ready_insert(task))
    {
        return false;
    }
    if (priority_higher_than_current(task))
    {
        hr_port_request_context_switch();
    }
    return true;
}

bool hr_task_suspend(hr_task_t *task)
{
    const hr_irq_state_t irq_state = hr_critical_enter();
    if (!hr_task_is_valid(task) || (task->state == HR_TASK_SUSPENDED) || (task->name[0] == 'i' && task->name[1] == 'd'
        && task->name[2] == 'l' && task->name[3] == 'e'))
    {
        hr_critical_exit(irq_state);
        return false;
    }
    if (task->ready_node.owner != (const void *)0)
    {
        (void) hr_ready_remove(task);
    }
    if (task->wait_node.owner != (const void *)0)
    {
        hr_list_t *list = (hr_list_t *) task->wait_node.owner;
        (void) hr_list_remove(list, &task->wait_node);
    }
    if (task->timeout_node.owner != (const void *)0)
    {
        (void) hr_timeout_remove(task);
    }
    if (task->wait_kind == HR_WAIT_KIND_MUTEX)
    {
        hr_mutex_waiter_removed_locked(task->waiting_object);
    }
    task->waiting_object = (void *)0;
    task->waiting_list = (hr_list_t *)0;
    task->wake_tick = HR_WAIT_FOREVER;
    task->wait_result = HR_WAIT_SUSPENDED;
    task->wait_kind = HR_WAIT_KIND_NONE;
    task->state = HR_TASK_SUSPENDED;
    if (task == g_current_task)
    {
        hr_port_request_context_switch();
    }
    hr_critical_exit(irq_state);
    return true;
}

bool hr_task_resume(hr_task_t *task)
{
    const hr_irq_state_t irq_state = hr_critical_enter();
    if (!hr_task_is_valid(task) || (task->state != HR_TASK_SUSPENDED))
    {
        hr_critical_exit(irq_state);
        return false;
    }
    task->wait_result = HR_WAIT_SUCCESS;
    task->state = HR_TASK_READY;
    if (!hr_ready_insert(task))
    {
        hr_critical_exit(irq_state);
        return false;
    }
    if (priority_higher_than_current(task))
    {
        hr_port_request_context_switch();
    }
    hr_critical_exit(irq_state);
    return true;
}

hr_task_t *hr_scheduler_current(void)
{
    return g_current_task;
}

uint32_t hr_scheduler_tick_now(void)
{
    return g_kernel_tick;
}

bool hr_scheduler_validate(void)
{
    if (!hr_task_registry_validate() || !hr_ready_validate() || !hr_timeout_validate())
    {
        return false;
    }
    for (size_t i = 0U; i < hr_task_registry_count(); ++i)
    {
        const hr_task_t *task = hr_task_registry_at(i);
        if (task == (const hr_task_t *)0)
        {
            return false;
        }
        if ((task->state == HR_TASK_BLOCKED) && (task->ready_node.owner != (const void *)0))
        {
            return false;
        }
        if ((task->state == HR_TASK_SUSPENDED) && ((task->ready_node.owner != (const void *)0) || (task->wait_node.owner != (const void *)0)
            || (task->timeout_node.owner != (const void *)0)))
        {
            return false;
        }
        if ((task->state != HR_TASK_BLOCKED) && ((task->waiting_object != (void *)0) || (task->waiting_list != (hr_list_t *)0)))
        {
            return false;
        }
        if (!hr_list_validate(&task->owned_mutexes))
        {
            return false;
        }
    }
    return (g_current_task == (hr_task_t *)0) || hr_task_is_valid(g_current_task);
}
