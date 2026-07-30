#ifndef TASK_APP_H
#define TASK_APP_H

#include "message.h"

void task_app_init(void);
void task_app_handler(const ak_message_t *message);

#endif
