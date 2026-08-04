#include "task_button.h"

#include "app_signals.h"
#include "app_tasks.h"
#include "button.h"
#include "task.h"
#include "timer.h"

static ak_timer_id_t g_debounce_timer;

void task_button_init(void)
{
    g_debounce_timer = AK_TIMER_ID_INVALID;
}

void task_button_handler(const ak_message_t *message)
{
    if (message->signal == APP_SIGNAL_BUTTON_EDGE)
    {
        if (g_debounce_timer != AK_TIMER_ID_INVALID)
        {
            (void) ak_timer_cancel(g_debounce_timer);
        }
        g_debounce_timer = ak_timer_start_one_shot(APP_TASK_BUTTON, APP_TASK_BUTTON, APP_SIGNAL_BUTTON_DEBOUNCE_TIMEOUT,
        20U);
    }
    else if (message->signal == APP_SIGNAL_BUTTON_DEBOUNCE_TIMEOUT)
    {
        g_debounce_timer = AK_TIMER_ID_INVALID;
        if (button_is_pressed())
        {
            ak_message_t *pressed = ak_message_create_pure(APP_TASK_BUTTON, APP_TASK_APPLICATION, APP_SIGNAL_BUTTON_PRESSED);
            if ((pressed != 0) && !ak_task_post(pressed))
            {
                ak_message_release(pressed);
            }
        }
    }
}
