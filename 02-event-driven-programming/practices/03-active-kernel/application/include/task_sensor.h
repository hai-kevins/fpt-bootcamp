#ifndef TASK_SENSOR_H
#define TASK_SENSOR_H

#include "message.h"

void task_sensor_init(void);
void task_sensor_handler(const ak_message_t *message);

#endif
