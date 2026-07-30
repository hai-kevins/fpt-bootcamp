#ifndef APP_H
#define APP_H

#include <stdint.h>

void app_init(void);
void app_post_start(void);
void app_isr_button_edge(void);
void app_isr_uart_byte(uint8_t byte);
uint8_t app_state_get(void);

#endif
