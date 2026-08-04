#ifndef CRITICAL_SECTION_H
#define CRITICAL_SECTION_H
#include <stdint.h>

typedef uint32_t hr_irq_state_t;

static inline hr_irq_state_t hr_critical_enter(void)
{
    return 0U;
}

static inline void hr_critical_exit(hr_irq_state_t s)
{
    (void) s;
}
#endif
