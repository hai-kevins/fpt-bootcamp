#ifndef PANIC_H
#define PANIC_H
#include <stdint.h>

enum
{
    RTOS_PANIC_TASK_RETURNED = 2
};

_Noreturn void rtos_panic(int code, uint32_t detail);
_Noreturn void rtos_assert_failed(uint32_t line);
#endif
