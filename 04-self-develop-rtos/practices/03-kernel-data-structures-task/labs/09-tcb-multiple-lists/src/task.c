#include "task.h"
#include "panic.h"
#include <stddef.h>
#include <stdint.h>

_Static_assert(offsetof(rtos_task_t, saved_sp) == 0U, "saved_sp must be the first TCB field");

bool rtos_task_create_static(rtos_task_t *task, const char *name, uint8_t id, uint8_t priority, rtos_task_entry_t entry,
    void *argument, uint32_t *stack, size_t stack_word_count)
{
    uint32_t *sp;
    if ((task == (rtos_task_t *)0) || (name == (const char *)0) || (entry == (rtos_task_entry_t)0) || (stack == (uint32_t *)0)
        || (stack_word_count < RTOS_MIN_STACK_WORDS) || (((uintptr_t) stack & 0x7U) != 0U) || (task->magic == RTOS_TASK_MAGIC))
    {
        return false;
    }

    for (size_t i = 0U; i < stack_word_count; ++i)
    {
        stack[i] = RTOS_TASK_STACK_FILL;
    }
    stack[0] = RTOS_TASK_STACK_GUARD;
    sp = stack + stack_word_count;
    sp = (uint32_t *)((uintptr_t) sp & ~(uintptr_t)0x7U);

    *(--sp) = 0x01000000UL;
    *(--sp) = (uint32_t)(uintptr_t) entry;
    *(--sp) = (uint32_t)(uintptr_t) rtos_task_return_error;
    *(--sp) = 0x12121212UL;
    *(--sp) = 0x03030303UL;
    *(--sp) = 0x02020202UL;
    *(--sp) = 0x01010101UL;
    *(--sp) = (uint32_t)(uintptr_t) argument;
    *(--sp) = 0x11111111UL;
    *(--sp) = 0x10101010UL;
    *(--sp) = 0x09090909UL;
    *(--sp) = 0x08080808UL;
    *(--sp) = 0x07070707UL;
    *(--sp) = 0x06060606UL;
    *(--sp) = 0x05050505UL;
    *(--sp) = 0x04040404UL;

    task->saved_sp = sp;
    task->stack_low = stack;
    task->stack_high = stack + stack_word_count;
    task->stack_word_count = stack_word_count;
    task->name = name;
    task->entry = entry;
    task->argument = argument;
    task->wake_tick = RTOS_WAIT_FOREVER;
    task->runtime_ticks = 0U;
    task->switch_count = 0U;
    task->time_slice_remaining = RTOS_DEFAULT_TIME_SLICE_TICKS;
    task->magic = RTOS_TASK_MAGIC;
    task->id = id;
    task->base_priority = priority;
    task->effective_priority = priority;
    task->state = RTOS_TASK_CREATED;
    rtos_list_node_init(&task->ready_node);
    rtos_list_node_init(&task->all_task_node);
    return rtos_task_saved_sp_is_valid(task);
}

bool rtos_task_is_valid(const rtos_task_t *task)
{
    return (task != (const rtos_task_t *)0) && (task->magic == RTOS_TASK_MAGIC) && (task->name != (const char *)0) && (task->entry != (rtos_task_entry_t)0) && rtos_task_saved_sp_is_valid(task);
}

bool rtos_task_saved_sp_is_valid(const rtos_task_t *task)
{
    if ((task == (const rtos_task_t *)0) || (task->saved_sp == (uint32_t *)0) || (task->stack_low == (uint32_t *)0)
        || (task->stack_high == (uint32_t *)0))
    {
        return false;
    }
    return (task->saved_sp >= task->stack_low) && (task->saved_sp < task->stack_high) && (((uintptr_t) task->saved_sp & 0x7U) == 0U);
}

size_t rtos_task_stack_unused_words(const rtos_task_t *task)
{
    size_t unused = 0U;
    const uint32_t *cursor;
    if (!rtos_task_is_valid(task))
    {
        return 0U;
    }
    cursor = task->stack_low + 1;
    while ((cursor < task->saved_sp) && (*cursor == RTOS_TASK_STACK_FILL))
    {
        ++unused;
        ++cursor;
    }
    return unused;
}

bool rtos_task_stack_guard_ok(const rtos_task_t *task)
{
    return rtos_task_is_valid(task) && (task->stack_low[0] == RTOS_TASK_STACK_GUARD);
}

void rtos_task_return_error(void)
{
    rtos_panic(RTOS_PANIC_TASK_RETURNED, 0U);
}
