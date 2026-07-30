#ifndef TASK_LED_H
#define TASK_LED_H

#include "message.h"

void task_led_init(void);
void task_led_handler(const ak_message_t *message);

#endif
