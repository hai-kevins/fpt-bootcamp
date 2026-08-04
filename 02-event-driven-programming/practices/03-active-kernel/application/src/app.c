#include "app.h"

#include "ak.h"
#include "app_signals.h"
#include "app_tasks.h"
#include "task_app.h"
#include "task_button.h"
#include "task_led.h"
#include "task_sensor.h"
#include "task_shell.h"

void app_init(void)
{
    task_app_init();
    task_button_init();
    task_led_init();
    task_shell_init();
    task_sensor_init();
    (void) app_tasks_register();
}

void app_post_start(void)
{
    ak_message_t *message = ak_message_create_pure(APP_TASK_NONE, APP_TASK_APPLICATION, APP_SIGNAL_SYSTEM_START);
    if ((message != 0) && !ak_task_post(message))
    {
        ak_message_release(message);
    }
}

void app_isr_button_edge(void)
{
    ak_message_t *message;
    ak_task_entry_interrupt();
    message = ak_message_create_pure(APP_TASK_NONE, APP_TASK_BUTTON, APP_SIGNAL_BUTTON_EDGE);
    if ((message != 0) && !ak_task_post_from_isr(message))
    {
        ak_message_release(message);
    }
    ak_task_exit_interrupt();
}

void app_isr_uart_byte(uint8_t byte)
{
    ak_message_t *message;
    ak_task_entry_interrupt();
    message = ak_message_create_common(APP_TASK_NONE, APP_TASK_SHELL, APP_SIGNAL_UART_RX_BYTE, &byte, sizeof(byte));
    if ((message != 0) && !ak_task_post_from_isr(message))
    {
        ak_message_release(message);
    }
    ak_task_exit_interrupt();
}
