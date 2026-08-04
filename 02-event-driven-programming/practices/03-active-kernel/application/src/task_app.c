#include "task_app.h"

#include "app.h"
#include "app_signals.h"
#include "app_tasks.h"
#include "fsm.h"
#include "message.h"
#include "task.h"

#include <stdint.h>

typedef enum
{
    APP_STATE_BOOT = 0,
    APP_STATE_IDLE,
    APP_STATE_ACTIVE,
    APP_STATE_ERROR
} app_state_t;

static ak_fsm_t g_fsm;

static void post_led(uint16_t signal)
{
    ak_message_t *message = ak_message_create_pure(APP_TASK_APPLICATION, APP_TASK_LED, signal);
    if ((message != 0) && !ak_task_post(message))
    {
        ak_message_release(message);
    }
}

static void post_blink(uint32_t period)
{
    ak_message_t *message = ak_message_create_common(APP_TASK_APPLICATION, APP_TASK_LED, APP_SIGNAL_LED_BLINK_START,
        &period, sizeof(period));
    if ((message != 0) && !ak_task_post(message))
    {
        ak_message_release(message);
    }
}

static bool app_dispatch(ak_fsm_t *fsm, const ak_message_t *message)
{
    switch ((app_state_t) fsm->state)
    {
        case APP_STATE_BOOT : if (message->signal == APP_SIGNAL_SYSTEM_START)
        {
            post_led(APP_SIGNAL_LED_OFF);
            ak_fsm_transition(fsm, APP_STATE_IDLE);
            return true;
        }
        break;

        case APP_STATE_IDLE : if (message->signal == APP_SIGNAL_BUTTON_PRESSED)
        {
            post_blink(500U);
            ak_fsm_transition(fsm, APP_STATE_ACTIVE);
            return true;
        }
        if (message->signal == APP_SIGNAL_APP_ERROR)
        {
            post_blink(100U);
            ak_fsm_transition(fsm, APP_STATE_ERROR);
            return true;
        }
        break;

        case APP_STATE_ACTIVE : if (message->signal == APP_SIGNAL_BUTTON_PRESSED)
        {
            post_led(APP_SIGNAL_LED_OFF);
            ak_fsm_transition(fsm, APP_STATE_IDLE);
            return true;
        }
        if (message->signal == APP_SIGNAL_APP_ERROR)
        {
            post_blink(100U);
            ak_fsm_transition(fsm, APP_STATE_ERROR);
            return true;
        }
        break;

        case APP_STATE_ERROR : if (message->signal == APP_SIGNAL_APP_RESET)
        {
            post_led(APP_SIGNAL_LED_OFF);
            ak_fsm_transition(fsm, APP_STATE_IDLE);
            return true;
        }
        break;

        default:
        break;
    }
    return false;
}

void task_app_init(void)
{
    ak_fsm_init(&g_fsm, APP_STATE_BOOT, 0, app_dispatch);
}

void task_app_handler(const ak_message_t *message)
{
    (void) ak_fsm_handle(&g_fsm, message);
}

uint8_t app_state_get(void)
{
    return g_fsm.state;
}
