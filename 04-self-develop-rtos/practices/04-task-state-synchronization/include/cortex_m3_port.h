#ifndef CORTEX_M3_PORT_H
#define CORTEX_M3_PORT_H

#include "compiler.h"
#include <stdint.h>

void rtos_port_init(void);
RTOS_NORETURN void rtos_port_start_first_task(void);
void rtos_port_request_context_switch(void);
void rtos_port_wait_for_interrupt(void);
uint32_t rtos_port_get_psp(void);
uint32_t rtos_port_get_msp(void);

#endif
