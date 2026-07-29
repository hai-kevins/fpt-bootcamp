#ifndef UART_SERVICE_H
#define UART_SERVICE_H

#include "event.h"

#include <stddef.h>

void uart_service_init(void);
void uart_service_event_handler(const event_t *event);
size_t uart_service_command_length(void);

#endif
