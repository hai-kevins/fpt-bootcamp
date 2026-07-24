#ifndef PANIC_H
#define PANIC_H

#include "compiler.h"
#include <stdint.h>

typedef enum
{
    HR_PANIC_NONE = 0,
    HR_PANIC_ASSERT = 1,
    HR_PANIC_TASK_RETURNED = 2,
    HR_PANIC_NO_READY_TASK = 3,
    HR_PANIC_STACK_CORRUPTION = 4,
    HR_PANIC_HARDFAULT = 5
} hr_panic_code_t;

extern volatile hr_panic_code_t g_hr_panic_code;
extern volatile uint32_t g_hr_panic_detail;

HR_NORETURN void hr_panic(hr_panic_code_t code, uint32_t detail);
HR_NORETURN void hr_assert_failed(uint32_t line);

#define HR_ASSERT(condition)     do     {         if (!(condition))         {             hr_assert_failed((uint32_t)__LINE__);         }     } while (0)

#endif
