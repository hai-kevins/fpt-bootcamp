#ifndef CRITICAL_SECTION_H
#define CRITICAL_SECTION_H

#include <stdint.h>

typedef uint32_t hr_irq_state_t;

hr_irq_state_t hr_critical_enter(void);
void hr_critical_exit(hr_irq_state_t state);

#endif
