#ifndef CRITICAL_SECTION_H
#define CRITICAL_SECTION_H

#include <stdint.h>

typedef uint32_t rtos_irq_state_t;

rtos_irq_state_t rtos_critical_enter(void);
void rtos_critical_exit(rtos_irq_state_t state);

#endif
