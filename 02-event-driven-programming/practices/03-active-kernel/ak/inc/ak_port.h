#ifndef AK_PORT_H
#define AK_PORT_H

#include <stdint.h>

uint32_t ak_port_critical_enter(void);
void ak_port_critical_exit(uint32_t previous_state);
uint32_t ak_port_time_now_ms(void);
void ak_port_reset(void);

#endif
