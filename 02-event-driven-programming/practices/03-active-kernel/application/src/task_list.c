#include "app_tasks.h"

#include "task.h"
#include "task_app.h"
#include "task_button.h"
#include "task_led.h"
#include "task_sensor.h"
#include "task_shell.h"

bool app_tasks_register(void)
{
    bool ok = true;
    ok = ak_task_register(APP_TASK_APPLICATION, 4U, "app", task_app_handler) && ok;
    ok = ak_task_register(APP_TASK_BUTTON, 6U, "button", task_button_handler) && ok;
    ok = ak_task_register(APP_TASK_LED, 3U, "led", task_led_handler) && ok;
    ok = ak_task_register(APP_TASK_SHELL, 2U, "shell", task_shell_handler) && ok;
    ok = ak_task_register(APP_TASK_SENSOR, 1U, "sensor", task_sensor_handler) && ok;
    return ok;
}
