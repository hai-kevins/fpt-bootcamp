#include "scheduler.h"
#include "cortex_m3_port.h"
#include "critical_section.h"
#include "mutex.h"
#include "panic.h"
#include "ready_queue.h"
#include "task_registry.h"
#include "timeout.h"
#include "wait_list.h"

rtos_task_t *g_current_task;
volatile uint32_t g_kernel_tick;
volatile uint32_t g_context_switch_count;

static bool priority_higher_than_current(const rtos_task_t *task)
{
    return (task != (const rtos_task_t *)0) &&
           ((g_current_task == (rtos_task_t *)0) ||
            (task->effective_priority < g_current_task->effective_priority));
}

void rtos_scheduler_init(void)
{
    rtos_task_registry_init();
    rtos_ready_init();
    rtos_timeout_init();
    g_current_task = (rtos_task_t *)0;
    g_kernel_tick = 0U;
    g_context_switch_count = 0U;
}

bool rtos_scheduler_add_task(rtos_task_t *task)
{
    const rtos_irq_state_t state = rtos_critical_enter();
    bool ok = false;
    if (rtos_task_is_valid(task) && (task->state == RTOS_TASK_CREATED) &&
        (task->effective_priority < RTOS_PRIORITY_COUNT))
    {
        ok = rtos_task_registry_add(task) && rtos_ready_insert(task);
    }
    rtos_critical_exit(state);
    return ok;
}

void rtos_scheduler_start(void)
{
    rtos_port_init();
    g_current_task = rtos_ready_select_highest();
    if (g_current_task == (rtos_task_t *)0) { rtos_panic(RTOS_PANIC_NO_READY_TASK, 0U); }
    g_current_task->state = RTOS_TASK_RUNNING;
    ++g_current_task->switch_count;
    rtos_port_start_first_task();
}

void rtos_scheduler_commit_switch(void)
{
    rtos_task_t *old_task = g_current_task;
    rtos_task_t *next_task = rtos_ready_select_highest();
    if (next_task == (rtos_task_t *)0) { rtos_panic(RTOS_PANIC_NO_READY_TASK, rtos_ready_bitmap()); }
    if ((old_task != (rtos_task_t *)0) && (old_task != next_task) &&
        (old_task->state == RTOS_TASK_RUNNING)) { old_task->state = RTOS_TASK_READY; }
    next_task->state = RTOS_TASK_RUNNING;
    g_current_task = next_task;
    if (old_task != next_task)
    {
        ++g_context_switch_count;
        ++next_task->switch_count;
    }
}

bool rtos_scheduler_wake_task_locked(rtos_task_t *task, rtos_wait_result_t result)
{
    if (!rtos_task_is_valid(task) || (task->state != RTOS_TASK_BLOCKED)) { return false; }

    if (task->wait_node.owner != (const void *)0)
    {
        rtos_list_t *list = (rtos_list_t *)task->wait_node.owner;
        if (!rtos_list_remove(list, &task->wait_node)) { return false; }
    }
    if (task->timeout_node.owner != (const void *)0)
    {
        if (!rtos_timeout_remove(task)) { return false; }
    }

    task->waiting_object = (void *)0;
    task->waiting_list = (rtos_list_t *)0;
    task->wake_tick = RTOS_WAIT_FOREVER;
    task->wait_result = result;
    task->wait_kind = RTOS_WAIT_KIND_NONE;
    task->state = RTOS_TASK_READY;
    if (!rtos_ready_insert(task)) { return false; }
    if (priority_higher_than_current(task)) { rtos_port_request_context_switch(); }
    return true;
}

bool rtos_scheduler_block_current_locked(void *object,
                                       rtos_list_t *waiters,
                                       rtos_wait_kind_t kind,
                                       uint32_t timeout_ticks)
{
    rtos_task_t *task = g_current_task;
    if (!rtos_task_is_valid(task) || (task->state != RTOS_TASK_RUNNING) ||
        (task->name[0] == 'i' && task->name[1] == 'd' && task->name[2] == 'l' && task->name[3] == 'e'))
    {
        return false;
    }

    if (!rtos_ready_remove(task)) { return false; }
    task->waiting_object = object;
    task->waiting_list = waiters;
    task->wait_result = RTOS_WAIT_NONE;
    task->wait_kind = kind;
    task->wake_tick = RTOS_WAIT_FOREVER;
    task->state = RTOS_TASK_BLOCKED;

    if ((waiters != (rtos_list_t *)0) && !rtos_wait_list_insert(waiters, task))
    {
        task->state = RTOS_TASK_READY;
        task->waiting_object = (void *)0;
        task->waiting_list = (rtos_list_t *)0;
        (void)rtos_ready_insert(task);
        return false;
    }

    if ((timeout_ticks != RTOS_WAIT_FOREVER) && (timeout_ticks != RTOS_NO_WAIT))
    {
        task->wake_tick = g_kernel_tick + timeout_ticks;
        if (!rtos_timeout_insert(task))
        {
            if (task->wait_node.owner != (const void *)0)
            {
                (void)rtos_list_remove(waiters, &task->wait_node);
            }
            task->state = RTOS_TASK_READY;
            task->waiting_object = (void *)0;
            task->waiting_list = (rtos_list_t *)0;
            task->wake_tick = RTOS_WAIT_FOREVER;
            (void)rtos_ready_insert(task);
            return false;
        }
    }

    rtos_port_request_context_switch();
    return true;
}

static void process_timeouts(void)
{
    rtos_task_t *task;
    while ((task = rtos_timeout_take_due(g_kernel_tick)) != (rtos_task_t *)0)
    {
        const rtos_wait_kind_t kind = task->wait_kind;
        void *object = task->waiting_object;
        if (task->wait_node.owner != (const void *)0)
        {
            rtos_list_t *list = (rtos_list_t *)task->wait_node.owner;
            (void)rtos_list_remove(list, &task->wait_node);
        }
        task->waiting_object = (void *)0;
        task->waiting_list = (rtos_list_t *)0;
        task->wake_tick = RTOS_WAIT_FOREVER;
        task->wait_result = (kind == RTOS_WAIT_KIND_DELAY) ? RTOS_WAIT_SUCCESS : RTOS_WAIT_TIMEOUT;
        task->wait_kind = RTOS_WAIT_KIND_NONE;
        task->state = RTOS_TASK_READY;
        if (!rtos_ready_insert(task)) { rtos_panic(RTOS_PANIC_ASSERT, (uint32_t)__LINE__); }
        if (kind == RTOS_WAIT_KIND_MUTEX) { rtos_mutex_waiter_removed_locked(object); }
        if (priority_higher_than_current(task)) { rtos_port_request_context_switch(); }
    }
}

void rtos_scheduler_on_tick(void)
{
    bool switch_required = false;
    const rtos_irq_state_t irq_state = rtos_critical_enter();
    ++g_kernel_tick;
    process_timeouts();

    if ((g_current_task != (rtos_task_t *)0) && (g_current_task->state == RTOS_TASK_RUNNING))
    {
        ++g_current_task->runtime_ticks;
        if (g_current_task->time_slice_remaining > 0U) { --g_current_task->time_slice_remaining; }
        if (g_current_task->time_slice_remaining == 0U)
        {
            g_current_task->time_slice_remaining = RTOS_DEFAULT_TIME_SLICE_TICKS;
            if (rtos_ready_count(g_current_task->effective_priority) > 1U)
            {
                g_current_task->state = RTOS_TASK_READY;
                if (!rtos_ready_rotate(g_current_task->effective_priority))
                { rtos_panic(RTOS_PANIC_ASSERT, (uint32_t)__LINE__); }
                switch_required = true;
            }
        }
    }
    if (switch_required) { rtos_port_request_context_switch(); }
    rtos_critical_exit(irq_state);
}

void rtos_task_yield(void)
{
    const rtos_irq_state_t irq_state = rtos_critical_enter();
    if ((g_current_task != (rtos_task_t *)0) &&
        (rtos_ready_count(g_current_task->effective_priority) > 1U))
    {
        g_current_task->state = RTOS_TASK_READY;
        (void)rtos_ready_rotate(g_current_task->effective_priority);
        rtos_port_request_context_switch();
    }
    rtos_critical_exit(irq_state);
}

rtos_wait_result_t rtos_task_delay(uint32_t ticks)
{
    rtos_task_t *task;
    const rtos_irq_state_t irq_state = rtos_critical_enter();
    if (ticks == 0U)
    {
        rtos_critical_exit(irq_state);
        rtos_task_yield();
        return RTOS_WAIT_SUCCESS;
    }
    task = g_current_task;
    if (!rtos_scheduler_block_current_locked((void *)0, (rtos_list_t *)0,
                                           RTOS_WAIT_KIND_DELAY, ticks))
    {
        rtos_critical_exit(irq_state);
        return RTOS_WAIT_CANCELLED;
    }
    rtos_critical_exit(irq_state);
    return task->wait_result;
}

rtos_wait_result_t rtos_task_delay_until(uint32_t *previous_wake, uint32_t period_ticks)
{
    uint32_t now;
    uint32_t next;
    if ((previous_wake == (uint32_t *)0) || (period_ticks == 0U))
    {
        return RTOS_WAIT_CANCELLED;
    }
    now = g_kernel_tick;
    next = *previous_wake + period_ticks;
    *previous_wake = next;
    if (rtos_tick_reached(now, next)) { return RTOS_WAIT_SUCCESS; }
    return rtos_task_delay(next - now);
}

bool rtos_scheduler_set_effective_priority_locked(rtos_task_t *task, uint8_t priority)
{
    bool linked;
    if (!rtos_task_is_valid(task) || (priority >= RTOS_PRIORITY_COUNT)) { return false; }
    if (task->effective_priority == priority) { return true; }
    linked = task->ready_node.owner != (const void *)0;
    if (linked && !rtos_ready_remove(task)) { return false; }
    task->effective_priority = priority;
    if (linked && !rtos_ready_insert(task)) { return false; }
    if (priority_higher_than_current(task)) { rtos_port_request_context_switch(); }
    return true;
}

bool rtos_task_suspend(rtos_task_t *task)
{
    const rtos_irq_state_t irq_state = rtos_critical_enter();
    if (!rtos_task_is_valid(task) || (task->state == RTOS_TASK_SUSPENDED) ||
        (task->name[0] == 'i' && task->name[1] == 'd' && task->name[2] == 'l' && task->name[3] == 'e'))
    {
        rtos_critical_exit(irq_state);
        return false;
    }
    if (task->ready_node.owner != (const void *)0) { (void)rtos_ready_remove(task); }
    if (task->wait_node.owner != (const void *)0)
    {
        rtos_list_t *list = (rtos_list_t *)task->wait_node.owner;
        (void)rtos_list_remove(list, &task->wait_node);
    }
    if (task->timeout_node.owner != (const void *)0) { (void)rtos_timeout_remove(task); }
    if (task->wait_kind == RTOS_WAIT_KIND_MUTEX) { rtos_mutex_waiter_removed_locked(task->waiting_object); }
    task->waiting_object = (void *)0;
    task->waiting_list = (rtos_list_t *)0;
    task->wake_tick = RTOS_WAIT_FOREVER;
    task->wait_result = RTOS_WAIT_SUSPENDED;
    task->wait_kind = RTOS_WAIT_KIND_NONE;
    task->state = RTOS_TASK_SUSPENDED;
    if (task == g_current_task) { rtos_port_request_context_switch(); }
    rtos_critical_exit(irq_state);
    return true;
}

bool rtos_task_resume(rtos_task_t *task)
{
    const rtos_irq_state_t irq_state = rtos_critical_enter();
    if (!rtos_task_is_valid(task) || (task->state != RTOS_TASK_SUSPENDED))
    {
        rtos_critical_exit(irq_state);
        return false;
    }
    task->wait_result = RTOS_WAIT_SUCCESS;
    task->state = RTOS_TASK_READY;
    if (!rtos_ready_insert(task))
    {
        rtos_critical_exit(irq_state);
        return false;
    }
    if (priority_higher_than_current(task)) { rtos_port_request_context_switch(); }
    rtos_critical_exit(irq_state);
    return true;
}

rtos_task_t *rtos_scheduler_current(void) { return g_current_task; }
uint32_t rtos_scheduler_tick_now(void) { return g_kernel_tick; }

bool rtos_scheduler_validate(void)
{
    if (!rtos_task_registry_validate() || !rtos_ready_validate() || !rtos_timeout_validate())
    { return false; }
    for (size_t i = 0U; i < rtos_task_registry_count(); ++i)
    {
        const rtos_task_t *task = rtos_task_registry_at(i);
        if (task == (const rtos_task_t *)0) { return false; }
        if ((task->state == RTOS_TASK_BLOCKED) &&
            (task->ready_node.owner != (const void *)0)) { return false; }
        if ((task->state == RTOS_TASK_SUSPENDED) &&
            ((task->ready_node.owner != (const void *)0) ||
             (task->wait_node.owner != (const void *)0) ||
             (task->timeout_node.owner != (const void *)0))) { return false; }
        if ((task->state != RTOS_TASK_BLOCKED) &&
            ((task->waiting_object != (void *)0) ||
             (task->waiting_list != (rtos_list_t *)0))) { return false; }
        if (!rtos_list_validate(&task->owned_mutexes)) { return false; }
    }
    return (g_current_task == (rtos_task_t *)0) || rtos_task_is_valid(g_current_task);
}
