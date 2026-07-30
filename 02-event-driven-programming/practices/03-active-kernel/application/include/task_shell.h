#ifndef TASK_SHELL_H
#define TASK_SHELL_H

#include "message.h"

void task_shell_init(void);
void task_shell_handler(const ak_message_t *message);

#endif
