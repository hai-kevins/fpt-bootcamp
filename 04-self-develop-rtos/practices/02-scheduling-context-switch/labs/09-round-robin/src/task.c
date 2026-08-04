#include "task.h"
#include "panic.h"

#include <stddef.h>
#include <stdint.h>

_Static_assert(offsetof(rtos_task_t, saved_sp) == 0U, "saved_sp must be the first TCB field");

bool rtos_task_create_static(rtos_task_t *task, const char *name, uint8_t id, uint8_t priority, rtos_task_entry_t entry,
    void *argument, uint32_t *stack, size_t stack_word_count)
{
    uint32_t *sp;

    if ((task == (rtos_task_t *)0) || (entry == (rtos_task_entry_t)0) || (stack == (uint32_t *)0) || (stack_word_count < 40U)
        || (stack_word_count > 65535U))
    {
        return false;
    }

    for (size_t index = 0U; index < stack_word_count; ++index)
    {
        stack[index] = RTOS_TASK_STACK_FILL;
    }
    stack[0] = RTOS_TASK_STACK_GUARD;

    sp = stack + stack_word_count;
    sp = (uint32_t *)((uintptr_t) sp & ~(uintptr_t)0x7U);

    *(--sp) = 0x01000000UL;
    /* xPSR */ * (--sp) = (uint32_t)(uintptr_t) entry;
    /* PC */ * (--sp) = (uint32_t)(uintptr_t) rtos_task_return_error;
    /* LR */ * (--sp) = 0x12121212UL;
    /* R12 */ * (--sp) = 0x03030303UL;
    /* R3 */ * (--sp) = 0x02020202UL;
    /* R2 */ * (--sp) = 0x01010101UL;
    /* R1 */ * (--sp) = (uint32_t)(uintptr_t) argument;
    /* R0 */

    *(--sp) = 0x11111111UL;
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
    task->name = name;
    task->entry = entry;
    task->argument = argument;
    task->ready_previous = (rtos_task_t *)0;
    task->ready_next = (rtos_task_t *)0;
    task->wake_tick = RTOS_WAIT_FOREVER;
    task->runtime_ticks = 0U;
    task->switch_count = 0U;
    task->time_slice_remaining = RTOS_DEFAULT_TIME_SLICE_TICKS;
    task->stack_word_count = (uint16_t) stack_word_count;
    task->id = id;
    task->priority = priority;
    task->state = RTOS_TASK_CREATED;
    task->in_ready_queue = false;

    return true;
}

size_t rtos_task_stack_unused_words(const rtos_task_t *task)
{
    size_t unused = 0U;
    const uint32_t *cursor;

    if ((task == (const rtos_task_t *)0) || (task->stack_low == (uint32_t *)0))
    {
        return 0U;
    }

    cursor = task->stack_low + 1;
    while ((cursor < task->stack_high) && (*cursor == RTOS_TASK_STACK_FILL))
    {
        ++unused;
        ++cursor;
    }

    return unused;
}

bool rtos_task_stack_guard_ok(const rtos_task_t *task)
{
    return (task != (const rtos_task_t *)0) && (task->stack_low != (uint32_t *)0) && (task->stack_low[0] == RTOS_TASK_STACK_GUARD);
}

void rtos_task_return_error(void)
{
    rtos_panic(RTOS_PANIC_TASK_RETURNED, 0U);
}
