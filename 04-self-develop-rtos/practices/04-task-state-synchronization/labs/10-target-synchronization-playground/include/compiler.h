#ifndef COMPILER_H
#define COMPILER_H

#define HR_NORETURN __attribute__((noreturn))
#define HR_NAKED __attribute__((naked))
#define HR_USED __attribute__((used))
#define HR_ALIGNED(value) __attribute__((aligned(value)))
#define HR_WEAK __attribute__((weak))

#endif
