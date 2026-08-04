#include "initial_stack.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

static void task_entry(void *argument)
{
    (void) argument;
}

int main(void)
{
    uint32_t stack[65];
    uint32_t argument = 0xCAFEBABEUL;
    const uintptr_t return_handler = (uintptr_t)0x08000101UL;
    uint32_t *sp;

    assert(lab_build_initial_stack(NULL, 64U, task_entry, &argument, return_handler) == NULL);
    assert(lab_build_initial_stack(stack, 8U, task_entry, &argument, return_handler) == NULL);
    assert(lab_build_initial_stack(stack, 64U, NULL, &argument, return_handler) == NULL);

    sp = lab_build_initial_stack(stack + 1, 64U, task_entry, &argument, return_handler);
    assert(sp != NULL);
    assert(((uintptr_t) sp & 0x7U) == 0U);
    assert(sp[0] == 0x04040404UL);
    assert(sp[7] == 0x11111111UL);
    assert(sp[8] == (uint32_t)(uintptr_t)&argument);
    assert(sp[13] == (uint32_t) return_handler);
    assert(sp[14] == (uint32_t)(uintptr_t) task_entry);
    assert(sp[15] == 0x01000000UL);

    puts("Lab 02 tests: PASS");
    return 0;
}
