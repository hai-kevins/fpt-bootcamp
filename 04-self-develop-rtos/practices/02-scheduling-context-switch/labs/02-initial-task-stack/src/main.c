#include "initial_stack.h"
#include <stdio.h>

static void demo_task(void *argument)
{
    (void) argument;
}

int main(void)
{
    uint32_t stack[64];
    uint32_t argument = 0x12345678UL;
    uint32_t *sp = lab_build_initial_stack(stack, 64U, demo_task, &argument, (uintptr_t)0x08000101UL);

    if (sp == NULL)
    {
        return 1;
    }

    for (size_t index = 0U; index < 16U; ++index)
    {
        printf("frame[%02zu] = 0x%08X\n", index, sp[index]);
    }

    return 0;
}
