#include "scheduler.h"
#include "cortex_m3_port.h"
#include "critical_section.h"
#include "panic.h"
#include "ready_queue.h"
#include "task_registry.h"
#include <stdint.h>

hr_task_t *g_current_task;
volatile uint32_t g_kernel_tick;
volatile uint32_t g_context_switch_count;

static bool tick_due(uint32_t now, uint32_t wake_tick)
{
    return (int32_t)(now - wake_tick) >= 0;
}

void hr_scheduler_init(void)
{
    hr_task_registry_init();
    hr_ready_init();
    g_current_task = (hr_task_t *)0;
    g_kernel_tick = 0U;
    g_context_switch_count = 0U;
}

bool hr_scheduler_add_task(hr_task_t *task)
{
    const hr_irq_state_t state = hr_critical_enter();
    bool ok = false;
    if (hr_task_is_valid(task) && (task->state == HR_TASK_CREATED) &&
        (task->effective_priority < HR_PRIORITY_COUNT))
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
    if (g_current_task == (hr_task_t *)0) { hr_panic(HR_PANIC_NO_READY_TASK, 0U); }
    g_current_task->state = HR_TASK_RUNNING;
    ++g_current_task->switch_count;
    hr_port_start_first_task();
}

void hr_scheduler_commit_switch(void)
{
    hr_task_t *old_task = g_current_task;
    hr_task_t *next_task = hr_ready_select_highest();
    if (next_task == (hr_task_t *)0) { hr_panic(HR_PANIC_NO_READY_TASK, hr_ready_bitmap()); }
    if ((old_task != (hr_task_t *)0) && (old_task != next_task) &&
        (old_task->state == HR_TASK_RUNNING)) { old_task->state = HR_TASK_READY; }
    next_task->state = HR_TASK_RUNNING;
    g_current_task = next_task;
    if (old_task != next_task)
    {
        ++g_context_switch_count;
        ++next_task->switch_count;
    }
}

void hr_scheduler_on_tick(void)
{
    bool switch_required = false;
    const hr_irq_state_t irq_state = hr_critical_enter();
    ++g_kernel_tick;

    for (size_t i = 0U; i < hr_task_registry_count(); ++i)
    {
        hr_task_t *task = hr_task_registry_at(i);
        if ((task != (hr_task_t *)0) && (task->state == HR_TASK_BLOCKED) &&
            (task->wake_tick != HR_WAIT_FOREVER) && tick_due(g_kernel_tick, task->wake_tick))
        {
            task->wake_tick = HR_WAIT_FOREVER;
            if (!hr_ready_insert(task)) { hr_panic(HR_PANIC_ASSERT, (uint32_t)__LINE__); }
            if ((g_current_task == (hr_task_t *)0) ||
                (task->effective_priority < g_current_task->effective_priority)) { switch_required = true; }
        }
    }

    if ((g_current_task != (hr_task_t *)0) && (g_current_task->state == HR_TASK_RUNNING))
    {
        ++g_current_task->runtime_ticks;
        if (g_current_task->time_slice_remaining > 0U) { --g_current_task->time_slice_remaining; }
        if (g_current_task->time_slice_remaining == 0U)
        {
            g_current_task->time_slice_remaining = HR_DEFAULT_TIME_SLICE_TICKS;
            if (hr_ready_count(g_current_task->effective_priority) > 1U)
            {
                g_current_task->state = HR_TASK_READY;
                if (!hr_ready_rotate(g_current_task->effective_priority))
                { hr_panic(HR_PANIC_ASSERT, (uint32_t)__LINE__); }
                switch_required = true;
            }
        }
    }
    if (switch_required) { hr_port_request_context_switch(); }
    hr_critical_exit(irq_state);
}

void hr_task_yield(void)
{
    const hr_irq_state_t irq_state = hr_critical_enter();
    if ((g_current_task != (hr_task_t *)0) &&
        (hr_ready_count(g_current_task->effective_priority) > 1U))
    {
        g_current_task->state = HR_TASK_READY;
        (void)hr_ready_rotate(g_current_task->effective_priority);
        hr_port_request_context_switch();
    }
    hr_critical_exit(irq_state);
}

void hr_task_delay(uint32_t ticks)
{
    if (ticks == 0U) { hr_task_yield(); return; }
    const hr_irq_state_t irq_state = hr_critical_enter();
    if ((g_current_task == (hr_task_t *)0) || !hr_ready_remove(g_current_task))
    { hr_panic(HR_PANIC_ASSERT, (uint32_t)__LINE__); }
    g_current_task->wake_tick = g_kernel_tick + ticks;
    g_current_task->state = HR_TASK_BLOCKED;
    hr_port_request_context_switch();
    hr_critical_exit(irq_state);
}

hr_task_t *hr_scheduler_current(void) { return g_current_task; }

bool hr_scheduler_validate(void)
{
    if (!hr_task_registry_validate() || !hr_ready_validate()) { return false; }
    if (g_current_task == (hr_task_t *)0) { return true; }
    return hr_task_is_valid(g_current_task) &&
           ((g_current_task->state == HR_TASK_RUNNING) ||
            (g_current_task->state == HR_TASK_BLOCKED));
}
