#include "scheduler.h"
#include "cortex_m3_port.h"
#include "critical_section.h"
#include "panic.h"
#include "ready_queue.h"
#include "task_registry.h"
#include <stdint.h>

rtos_task_t *g_current_task;
volatile uint32_t g_kernel_tick;
volatile uint32_t g_context_switch_count;

static bool tick_due(uint32_t now, uint32_t wake_tick)
{
    return (int32_t)(now - wake_tick) >= 0;
}

void rtos_scheduler_init(void)
{
    rtos_task_registry_init();
    rtos_ready_init();
    g_current_task = (rtos_task_t *)0;
    g_kernel_tick = 0U;
    g_context_switch_count = 0U;
}

bool rtos_scheduler_add_task(rtos_task_t *task)
{
    const rtos_irq_state_t state = rtos_critical_enter();
    bool ok = false;
    if (rtos_task_is_valid(task) && (task->state == RTOS_TASK_CREATED) && (task->effective_priority < RTOS_PRIORITY_COUNT))
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
    if (g_current_task == (rtos_task_t *)0)
    {
        rtos_panic(RTOS_PANIC_NO_READY_TASK, 0U);
    }
    g_current_task->state = RTOS_TASK_RUNNING;
    ++g_current_task->switch_count;
    rtos_port_start_first_task();
}

void rtos_scheduler_commit_switch(void)
{
    rtos_task_t *old_task = g_current_task;
    rtos_task_t *next_task = rtos_ready_select_highest();
    if (next_task == (rtos_task_t *)0)
    {
        rtos_panic(RTOS_PANIC_NO_READY_TASK, rtos_ready_bitmap());
    }
    if ((old_task != (rtos_task_t *)0) && (old_task != next_task) && (old_task->state == RTOS_TASK_RUNNING))
    {
        old_task->state = RTOS_TASK_READY;
    }
    next_task->state = RTOS_TASK_RUNNING;
    g_current_task = next_task;
    if (old_task != next_task)
    {
        ++g_context_switch_count;
        ++next_task->switch_count;
    }
}

void rtos_scheduler_on_tick(void)
{
    bool switch_required = false;
    const rtos_irq_state_t irq_state = rtos_critical_enter();
    ++g_kernel_tick;

    for (size_t i = 0U; i < rtos_task_registry_count(); ++i)
    {
        rtos_task_t *task = rtos_task_registry_at(i);
        if ((task != (rtos_task_t *)0) && (task->state == RTOS_TASK_BLOCKED) && (task->wake_tick != RTOS_WAIT_FOREVER)
            && tick_due(g_kernel_tick, task->wake_tick))
        {
            task->wake_tick = RTOS_WAIT_FOREVER;
            if (!rtos_ready_insert(task))
            {
                rtos_panic(RTOS_PANIC_ASSERT, (uint32_t) __LINE__);
            }
            if ((g_current_task == (rtos_task_t *)0) || (task->effective_priority < g_current_task->effective_priority))
            {
                switch_required = true;
            }
        }
    }

    if ((g_current_task != (rtos_task_t *)0) && (g_current_task->state == RTOS_TASK_RUNNING))
    {
        ++g_current_task->runtime_ticks;
        if (g_current_task->time_slice_remaining > 0U)
        {
            --g_current_task->time_slice_remaining;
        }
        if (g_current_task->time_slice_remaining == 0U)
        {
            g_current_task->time_slice_remaining = RTOS_DEFAULT_TIME_SLICE_TICKS;
            if (rtos_ready_count(g_current_task->effective_priority) > 1U)
            {
                g_current_task->state = RTOS_TASK_READY;
                if (!rtos_ready_rotate(g_current_task->effective_priority))
                {
                    rtos_panic(RTOS_PANIC_ASSERT, (uint32_t) __LINE__);
                }
                switch_required = true;
            }
        }
    }
    if (switch_required)
    {
        rtos_port_request_context_switch();
    }
    rtos_critical_exit(irq_state);
}

void rtos_task_yield(void)
{
    const rtos_irq_state_t irq_state = rtos_critical_enter();
    if ((g_current_task != (rtos_task_t *)0) && (rtos_ready_count(g_current_task->effective_priority) > 1U))
    {
        g_current_task->state = RTOS_TASK_READY;
        (void) rtos_ready_rotate(g_current_task->effective_priority);
        rtos_port_request_context_switch();
    }
    rtos_critical_exit(irq_state);
}

void rtos_task_delay(uint32_t ticks)
{
    if (ticks == 0U)
    {
        rtos_task_yield();
        return;
    }
    const rtos_irq_state_t irq_state = rtos_critical_enter();
    if ((g_current_task == (rtos_task_t *)0) || !rtos_ready_remove(g_current_task))
    {
        rtos_panic(RTOS_PANIC_ASSERT, (uint32_t) __LINE__);
    }
    g_current_task->wake_tick = g_kernel_tick + ticks;
    g_current_task->state = RTOS_TASK_BLOCKED;
    rtos_port_request_context_switch();
    rtos_critical_exit(irq_state);
}

rtos_task_t *rtos_scheduler_current(void)
{
    return g_current_task;
}

bool rtos_scheduler_validate(void)
{
    if (!rtos_task_registry_validate() || !rtos_ready_validate())
    {
        return false;
    }
    if (g_current_task == (rtos_task_t *)0)
    {
        return true;
    }
    return rtos_task_is_valid(g_current_task) && ((g_current_task->state == RTOS_TASK_RUNNING) || (g_current_task->state == RTOS_TASK_BLOCKED));
}
