#include "uart.h"
#include "platform.h"

void uart_init(void) { platform_uart_init(); }
void uart_write_byte(uint8_t byte) { platform_uart_write_byte(byte); }
void uart_write_string(const char *text) { platform_uart_write_string(text); }
