#ifndef APP_TASKS_H
#define APP_TASKS_H

#include <stdbool.h>
#include <stdint.h>

typedef enum
{
    APP_TASK_NONE = 0,
    APP_TASK_APPLICATION = 1,
    APP_TASK_BUTTON = 2,
    APP_TASK_LED = 3,
    APP_TASK_SHELL = 4,
    APP_TASK_SENSOR = 5
} app_task_id_t;

bool app_tasks_register(void);

#endif
