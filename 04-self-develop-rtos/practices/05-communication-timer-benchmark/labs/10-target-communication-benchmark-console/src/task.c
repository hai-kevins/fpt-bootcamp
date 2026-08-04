#include "task.h"
#include "panic.h"
#include <stddef.h>
#include <stdint.h>

_Static_assert(offsetof(hr_task_t, saved_sp) == 0U, "saved_sp must be the first TCB field");

bool hr_task_create_static(hr_task_t *task, const char *name, uint8_t id, uint8_t priority, hr_task_entry_t entry,
    void *argument, uint32_t *stack, size_t stack_word_count)
{
    uint32_t *sp;
    if ((task == (hr_task_t *)0) || (name == (const char *)0) || (entry == (hr_task_entry_t)0) || (stack == (uint32_t *)0)
        || (stack_word_count < HR_MIN_STACK_WORDS) || (((uintptr_t) stack & 0x7U) != 0U) || (task->magic == HR_TASK_MAGIC))
    {
        return false;
    }

    for (size_t i = 0U; i < stack_word_count; ++i)
    {
        stack[i] = HR_TASK_STACK_FILL;
    }
    stack[0] = HR_TASK_STACK_GUARD;
    sp = stack + stack_word_count;
    sp = (uint32_t *)((uintptr_t) sp & ~(uintptr_t)0x7U);

    *(--sp) = 0x01000000UL;
    /* xPSR */ * (--sp) = (uint32_t)(uintptr_t) entry;
    /* PC */ * (--sp) = (uint32_t)(uintptr_t) hr_task_return_error;
    /* LR */ * (--sp) = 0x12121212UL;
    /* R12 */ * (--sp) = 0x03030303UL;
    /* R3 */ * (--sp) = 0x02020202UL;
    /* R2 */ * (--sp) = 0x01010101UL;
    /* R1 */ * (--sp) = (uint32_t)(uintptr_t) argument;
    /* R0 */ * (--sp) = 0x11111111UL;
    /* R11 */ * (--sp) = 0x10101010UL;
    /* R10 */ * (--sp) = 0x09090909UL;
    /* R9 */ * (--sp) = 0x08080808UL;
    /* R8 */ * (--sp) = 0x07070707UL;
    /* R7 */ * (--sp) = 0x06060606UL;
    /* R6 */ * (--sp) = 0x05050505UL;
    /* R5 */ * (--sp) = 0x04040404UL;
    /* R4 */

    task->saved_sp = sp;
    task->stack_low = stack;
    task->stack_high = stack + stack_word_count;
    task->stack_word_count = stack_word_count;
    task->name = name;
    task->entry = entry;
    task->argument = argument;
    task->waiting_object = (void *)0;
    task->waiting_list = (hr_list_t *)0;
    task->wake_tick = HR_WAIT_FOREVER;
    task->runtime_ticks = 0U;
    task->switch_count = 0U;
    task->time_slice_remaining = HR_DEFAULT_TIME_SLICE_TICKS;
    task->magic = HR_TASK_MAGIC;
    task->id = id;
    task->base_priority = priority;
    task->effective_priority = priority;
    task->state = HR_TASK_CREATED;
    task->wait_result = HR_WAIT_NONE;
    task->wait_kind = HR_WAIT_KIND_NONE;
    hr_list_node_init(&task->ready_node);
    hr_list_node_init(&task->wait_node);
    hr_list_node_init(&task->timeout_node);
    hr_list_node_init(&task->all_task_node);
    hr_list_init(&task->owned_mutexes);
    return hr_task_saved_sp_is_valid(task);
}

bool hr_task_is_valid(const hr_task_t *task)
{
    return (task != (const hr_task_t *)0) && (task->magic == HR_TASK_MAGIC) && (task->name != (const char *)0) && (task->entry != (hr_task_entry_t)0) && hr_task_saved_sp_is_valid(task);
}

bool hr_task_saved_sp_is_valid(const hr_task_t *task)
{
    if ((task == (const hr_task_t *)0) || (task->saved_sp == (uint32_t *)0) || (task->stack_low == (uint32_t *)0)
        || (task->stack_high == (uint32_t *)0))
    {
        return false;
    }
    return (task->saved_sp >= task->stack_low) && (task->saved_sp < task->stack_high) && (((uintptr_t) task->saved_sp & 0x7U) == 0U);
}

size_t hr_task_stack_unused_words(const hr_task_t *task)
{
    size_t unused = 0U;
    const uint32_t *cursor;
    if (!hr_task_is_valid(task))
    {
        return 0U;
    }
    cursor = task->stack_low + 1;
    while ((cursor < task->saved_sp) && (*cursor == HR_TASK_STACK_FILL))
    {
        ++unused;
        ++cursor;
    }
    return unused;
}

bool hr_task_stack_guard_ok(const hr_task_t *task)
{
    return hr_task_is_valid(task) && (task->stack_low[0] == HR_TASK_STACK_GUARD);
}

bool hr_task_transition_allowed(hr_task_state_t from, hr_task_state_t to)
{
    switch (from)
    {
        case HR_TASK_CREATED : return to == HR_TASK_READY;
        case HR_TASK_READY : return (to == HR_TASK_RUNNING) || (to == HR_TASK_SUSPENDED);
        case HR_TASK_RUNNING : return (to == HR_TASK_READY) || (to == HR_TASK_BLOCKED) || (to == HR_TASK_SUSPENDED);
        case HR_TASK_BLOCKED : return (to == HR_TASK_READY) || (to == HR_TASK_SUSPENDED);
        case HR_TASK_SUSPENDED : return to == HR_TASK_READY;
        default:
        return false;
    }
}

const char *hr_task_state_name(hr_task_state_t state)
{
    switch (state)
    {
        case HR_TASK_UNUSED : return"UNUSED";
        case HR_TASK_CREATED : return"CREATED";
        case HR_TASK_READY : return"READY";
        case HR_TASK_RUNNING : return"RUNNING";
        case HR_TASK_BLOCKED : return"BLOCKED";
        case HR_TASK_SUSPENDED : return"SUSPENDED";
        default:
        return"UNKNOWN";
    }
}

const char *hr_wait_result_name(hr_wait_result_t result)
{
    switch (result)
    {
        case HR_WAIT_NONE : return"NONE";
        case HR_WAIT_SUCCESS : return"SUCCESS";
        case HR_WAIT_TIMEOUT : return"TIMEOUT";
        case HR_WAIT_CANCELLED : return"CANCELLED";
        case HR_WAIT_SUSPENDED : return"SUSPENDED";
        default:
        return"UNKNOWN";
    }
}

void hr_task_return_error(void)
{
    hr_panic(HR_PANIC_TASK_RETURNED, 0U);
}
