#ifndef CORTEX_M3_PORT_H
#define CORTEX_M3_PORT_H

#include "compiler.h"
#include <stdint.h>

void hr_port_init(void);
HR_NORETURN void hr_port_start_first_task(void);
void hr_port_request_context_switch(void);
void hr_port_wait_for_interrupt(void);
uint32_t hr_port_get_psp(void);
uint32_t hr_port_get_msp(void);

#endif
