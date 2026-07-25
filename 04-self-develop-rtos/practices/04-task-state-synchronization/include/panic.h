#ifndef PANIC_H
#define PANIC_H

#include "compiler.h"
#include <stdint.h>

typedef enum
{
    RTOS_PANIC_NONE = 0,
    RTOS_PANIC_ASSERT = 1,
    RTOS_PANIC_TASK_RETURNED = 2,
    RTOS_PANIC_NO_READY_TASK = 3,
    RTOS_PANIC_STACK_CORRUPTION = 4,
    RTOS_PANIC_HARDFAULT = 5,
    RTOS_PANIC_SYNCHRONIZATION = 6
} rtos_panic_code_t;

extern volatile rtos_panic_code_t g_rtos_panic_code;
extern volatile uint32_t g_rtos_panic_detail;

RTOS_NORETURN void rtos_panic(rtos_panic_code_t code, uint32_t detail);
RTOS_NORETURN void rtos_assert_failed(uint32_t line);

#define RTOS_ASSERT(condition)     do     {         if (!(condition))         {             rtos_assert_failed((uint32_t)__LINE__);         }     } while (0)

#endif
