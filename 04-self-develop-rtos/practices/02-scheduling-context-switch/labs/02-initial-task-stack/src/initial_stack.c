#include "initial_stack.h"

uint32_t *lab_build_initial_stack(uint32_t *stack, size_t word_count, lab_task_entry_t entry, void *argument, uintptr_t return_handler)
{
    uint32_t *sp;

    if ((stack == (uint32_t *)0) || (word_count < 32U) || (entry == (lab_task_entry_t)0) || (return_handler == (uintptr_t)0U))
    {
        return (uint32_t *)0;
    }

    sp = stack + word_count;
    sp = (uint32_t *)((uintptr_t) sp & ~(uintptr_t)0x7U);

    *(--sp) = 0x01000000UL;
    *(--sp) = (uint32_t)(uintptr_t) entry;
    *(--sp) = (uint32_t) return_handler;
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

    return sp;
}
