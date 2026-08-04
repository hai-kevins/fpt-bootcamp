#include "scheduler.h"
#include "cortex_m3_port.h"
#include "critical_section.h"
#include "panic.h"
#include "scheduler_trace.h"

#include <stdint.h>

typedef struct
{
    rtos_task_t *head;
    rtos_task_t *tail;
    uint8_t count;
} rtos_ready_queue_t;

static rtos_ready_queue_t g_ready_queues[RTOS_PRIORITY_COUNT];
static rtos_task_t *g_all_tasks[RTOS_MAX_TASKS];
static size_t g_task_count;
static uint32_t g_ready_bitmap;

rtos_task_t *g_current_task;
volatile uint32_t g_kernel_tick;
volatile uint32_t g_context_switch_count;

static bool tick_due(uint32_t now, uint32_t wake_tick)
{
    return (int32_t)(now - wake_tick) >= 0;
}

static uint8_t highest_ready_priority(void)
{
    for (uint8_t priority = 0U; priority < (uint8_t) RTOS_PRIORITY_COUNT; ++priority)
    {
        if ((g_ready_bitmap & (1UL << priority)) != 0U)
        {
            return priority;
        }
    }

    rtos_panic(RTOS_PANIC_NO_READY_TASK, g_ready_bitmap);
}

static void ready_set_bitmap(uint8_t priority)
{
    g_ready_bitmap |= (1UL << priority);
}

static void ready_clear_bitmap_if_empty(uint8_t priority)
{
    if (g_ready_queues[priority].count == 0U)
    {
        g_ready_bitmap &= ~ (1UL << priority);
    }
}

static void ready_enqueue_tail(rtos_task_t *task)
{
    rtos_ready_queue_t *queue;

    RTOS_ASSERT(task != (rtos_task_t *)0);
    RTOS_ASSERT(task->priority < RTOS_PRIORITY_COUNT);
    RTOS_ASSERT(!task->in_ready_queue);

    queue = &g_ready_queues[task->priority];
    task->ready_next = (rtos_task_t *)0;
    task->ready_previous = queue->tail;

    if (queue->tail != (rtos_task_t *)0)
    {
        queue->tail->ready_next = task;
    }
    else
    {
        queue->head = task;
    }

    queue->tail = task;
    ++queue->count;
    task->in_ready_queue = true;
    ready_set_bitmap(task->priority);
}

static void ready_remove(rtos_task_t *task)
{
    rtos_ready_queue_t *queue;

    RTOS_ASSERT(task != (rtos_task_t *)0);
    RTOS_ASSERT(task->priority < RTOS_PRIORITY_COUNT);
    RTOS_ASSERT(task->in_ready_queue);

    queue = &g_ready_queues[task->priority];

    if (task->ready_previous != (rtos_task_t *)0)
    {
        task->ready_previous->ready_next = task->ready_next;
    }
    else
    {
        queue->head = task->ready_next;
    }

    if (task->ready_next != (rtos_task_t *)0)
    {
        task->ready_next->ready_previous = task->ready_previous;
    }
    else
    {
        queue->tail = task->ready_previous;
    }

    task->ready_previous = (rtos_task_t *)0;
    task->ready_next = (rtos_task_t *)0;
    task->in_ready_queue = false;
    RTOS_ASSERT(queue->count > 0U);
    --queue->count;
    ready_clear_bitmap_if_empty(task->priority);
}

static void ready_rotate(uint8_t priority)
{
    rtos_ready_queue_t *queue = &g_ready_queues[priority];
    rtos_task_t *first;

    if (queue->count <= 1U)
    {
        return;
    }

    first = queue->head;
    RTOS_ASSERT(first != (rtos_task_t *)0);

    queue->head = first->ready_next;
    queue->head->ready_previous = (rtos_task_t *)0;

    first->ready_next = (rtos_task_t *)0;
    first->ready_previous = queue->tail;
    queue->tail->ready_next = first;
    queue->tail = first;
}

static rtos_task_t *select_next(void)
{
    const uint8_t priority = highest_ready_priority();
    rtos_task_t *next = g_ready_queues[priority].head;

    RTOS_ASSERT(next != (rtos_task_t *)0);
    RTOS_ASSERT(next->in_ready_queue);
    RTOS_ASSERT(rtos_task_stack_guard_ok(next));
    RTOS_ASSERT(next->saved_sp >= next->stack_low);
    RTOS_ASSERT(next->saved_sp < next->stack_high);

    return next;
}

void rtos_scheduler_init(void)
{
    for (size_t index = 0U; index < RTOS_PRIORITY_COUNT; ++index)
    {
        g_ready_queues[index].head = (rtos_task_t *)0;
        g_ready_queues[index].tail = (rtos_task_t *)0;
        g_ready_queues[index].count = 0U;
    }

    for (size_t index = 0U; index < RTOS_MAX_TASKS; ++index)
    {
        g_all_tasks[index] = (rtos_task_t *)0;
    }

    g_task_count = 0U;
    g_ready_bitmap = 0U;
    g_current_task = (rtos_task_t *)0;
    g_kernel_tick = 0U;
    g_context_switch_count = 0U;
    rtos_trace_reset();
}

bool rtos_scheduler_add_task(rtos_task_t *task)
{
    rtos_irq_state_t irq_state;

    if ((task == (rtos_task_t *)0) || (task->priority >= RTOS_PRIORITY_COUNT) || (task->state != RTOS_TASK_CREATED)
        || (g_task_count >= RTOS_MAX_TASKS))
    {
        return false;
    }

    irq_state = rtos_critical_enter();
    task->state = RTOS_TASK_READY;
    ready_enqueue_tail(task);
    g_all_tasks[g_task_count] = task;
    ++g_task_count;
    rtos_critical_exit(irq_state);
    return true;
}

void rtos_scheduler_start(void)
{
    rtos_port_init();
    g_current_task = select_next();
    g_current_task->state = RTOS_TASK_RUNNING;
    ++g_current_task->switch_count;
    rtos_trace_record(RTOS_TRACE_KERNEL_START, 0xFFU, g_current_task->id, g_kernel_tick);
    rtos_port_start_first_task();
}

void rtos_scheduler_commit_switch(void)
{
    rtos_task_t *old_task = g_current_task;
    rtos_task_t *next_task = select_next();

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
        rtos_trace_record(RTOS_TRACE_SWITCH, (old_task != (rtos_task_t *)0) ? old_task->id : 0xFFU, next_task->id,
        g_kernel_tick);
    }
}

void rtos_scheduler_on_tick(void)
{
    bool switch_required = false;
    const rtos_irq_state_t irq_state = rtos_critical_enter();

    ++g_kernel_tick;

    for (size_t index = 0U; index < g_task_count; ++index)
    {
        rtos_task_t *task = g_all_tasks[index];

        if ((task != (rtos_task_t *)0) && (task->state == RTOS_TASK_BLOCKED) && (task->wake_tick != RTOS_WAIT_FOREVER)
            && tick_due(g_kernel_tick, task->wake_tick))
        {
            task->wake_tick = RTOS_WAIT_FOREVER;
            task->state = RTOS_TASK_READY;
            ready_enqueue_tail(task);
            rtos_trace_record(RTOS_TRACE_WAKE, (g_current_task != (rtos_task_t *)0) ? g_current_task->id : 0xFFU,
                task->id, g_kernel_tick);

            if ((g_current_task == (rtos_task_t *)0) || (task->priority < g_current_task->priority))
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

            if (g_ready_queues[g_current_task->priority].count > 1U)
            {
                const uint8_t old_id = g_current_task->id;
                ready_rotate(g_current_task->priority);
                g_current_task->state = RTOS_TASK_READY;
                rtos_trace_record(RTOS_TRACE_TICK_ROTATE, old_id, g_ready_queues[g_current_task->priority].head->id,
                    g_kernel_tick);
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

    RTOS_ASSERT(g_current_task != (rtos_task_t *)0);
    RTOS_ASSERT(g_current_task->in_ready_queue);

    if (g_ready_queues[g_current_task->priority].count > 1U)
    {
        const uint8_t old_id = g_current_task->id;
        ready_rotate(g_current_task->priority);
        g_current_task->state = RTOS_TASK_READY;
        g_current_task->time_slice_remaining = RTOS_DEFAULT_TIME_SLICE_TICKS;
        rtos_trace_record(RTOS_TRACE_YIELD, old_id, g_ready_queues[g_current_task->priority].head->id, g_kernel_tick);
        rtos_port_request_context_switch();
    }

    rtos_critical_exit(irq_state);
}

void rtos_task_delay(uint32_t ticks)
{
    rtos_irq_state_t irq_state;

    if (ticks == 0U)
    {
        rtos_task_yield();
        return;
    }

    irq_state = rtos_critical_enter();
    RTOS_ASSERT(g_current_task != (rtos_task_t *)0);
    RTOS_ASSERT(g_current_task->in_ready_queue);

    ready_remove(g_current_task);
    g_current_task->wake_tick = g_kernel_tick + ticks;
    g_current_task->state = RTOS_TASK_BLOCKED;
    rtos_trace_record(RTOS_TRACE_DELAY, g_current_task->id, 0xFFU, g_kernel_tick);
    rtos_port_request_context_switch();
    rtos_critical_exit(irq_state);
}

void rtos_task_block_current(void)
{
    const rtos_irq_state_t irq_state = rtos_critical_enter();

    RTOS_ASSERT(g_current_task != (rtos_task_t *)0);
    RTOS_ASSERT(g_current_task->in_ready_queue);

    ready_remove(g_current_task);
    g_current_task->wake_tick = RTOS_WAIT_FOREVER;
    g_current_task->state = RTOS_TASK_BLOCKED;
    rtos_port_request_context_switch();
    rtos_critical_exit(irq_state);
}

bool rtos_task_wake(rtos_task_t *task)
{
    bool switch_required = false;
    const rtos_irq_state_t irq_state = rtos_critical_enter();

    if ((task != (rtos_task_t *)0) && (task->state == RTOS_TASK_BLOCKED))
    {
        task->wake_tick = RTOS_WAIT_FOREVER;
        task->state = RTOS_TASK_READY;
        ready_enqueue_tail(task);
        switch_required = (g_current_task == (rtos_task_t *)0) || (task->priority < g_current_task->priority);
        if (switch_required)
        {
            rtos_port_request_context_switch();
        }
    }

    rtos_critical_exit(irq_state);
    return switch_required;
}

bool rtos_task_wake_from_isr(rtos_task_t *task)
{
    bool switch_required = false;
    const rtos_irq_state_t irq_state = rtos_critical_enter();

    if ((task != (rtos_task_t *)0) && (task->state == RTOS_TASK_BLOCKED))
    {
        task->wake_tick = RTOS_WAIT_FOREVER;
        task->state = RTOS_TASK_READY;
        ready_enqueue_tail(task);
        switch_required = (g_current_task == (rtos_task_t *)0) || (task->priority < g_current_task->priority);
        rtos_trace_record(RTOS_TRACE_ISR_WAKE, (g_current_task != (rtos_task_t *)0) ? g_current_task->id : 0xFFU,
        task->id, g_kernel_tick);
        if (switch_required)
        {
            rtos_port_request_context_switch();
        }
    }

    rtos_critical_exit(irq_state);
    return switch_required;
}

rtos_task_t *rtos_scheduler_current(void)
{
    return g_current_task;
}

size_t rtos_scheduler_task_count(void)
{
    return g_task_count;
}

rtos_task_t *rtos_scheduler_task_at(size_t index)
{
    if (index >= g_task_count)
    {
        return (rtos_task_t *)0;
    }
    return g_all_tasks[index];
}

uint32_t rtos_scheduler_ready_bitmap(void)
{
    return g_ready_bitmap;
}
