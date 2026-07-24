#ifndef PANIC_H
#define PANIC_H
#include <stdint.h>
enum{HR_PANIC_TASK_RETURNED=2};
_Noreturn void hr_panic(int code,uint32_t detail);
_Noreturn void hr_assert_failed(uint32_t line);
#endif
