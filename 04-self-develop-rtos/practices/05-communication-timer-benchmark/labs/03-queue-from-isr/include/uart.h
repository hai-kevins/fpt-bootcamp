#ifndef UART_H
#define UART_H

#include <stdbool.h>
#include <stdint.h>

void uart1_init_9600_hsi8(void);
void uart1_write_byte(uint8_t byte);
void uart1_write_string(const char *text);
void uart1_write_hex32(uint32_t value);
void uart1_write_u32(uint32_t value);
bool uart1_byte_available(void);
uint8_t uart1_read_byte(void);

#endif
