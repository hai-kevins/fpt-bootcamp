#ifndef DRIVER_UART_H
#define DRIVER_UART_H

#include <stdint.h>

void uart_init(void);
void uart_write_byte(uint8_t byte);
void uart_write_string(const char *text);

#endif
