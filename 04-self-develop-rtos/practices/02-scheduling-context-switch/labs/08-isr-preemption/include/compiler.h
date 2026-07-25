#ifndef COMPILER_H
#define COMPILER_H

#define RTOS_NORETURN __attribute__((noreturn))
#define RTOS_NAKED __attribute__((naked))
#define RTOS_USED __attribute__((used))
#define RTOS_ALIGNED(value) __attribute__((aligned(value)))
#define RTOS_WEAK __attribute__((weak))

#endif
