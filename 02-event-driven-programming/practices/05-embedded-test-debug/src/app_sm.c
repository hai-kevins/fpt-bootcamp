#include "app_sm.h"

#include <stddef.h>

void app_sm_init(app_sm_t *app)
{
    if (app != NULL)
    {
        app->state = APP_STATE_IDLE;
        app->transition_count = 0U;
        app->unhandled_count = 0U;
        app->last_signal = EVENT_SIGNAL_NONE;
    }
}

bool app_sm_dispatch(app_sm_t *app, const event_t *event)
{
    if ((app == NULL) || (event == NULL))
    {
        return false;
    }

    app->last_signal = event->signal;

    switch (app->state)
    {
        case APP_STATE_IDLE : if (event->signal == EVENT_SIGNAL_APP_START)
        {
            app->state = APP_STATE_RUNNING;
            app->transition_count++;
            return true;
        }
        break;

        case APP_STATE_RUNNING : if (event->signal == EVENT_SIGNAL_APP_STOP)
        {
            app->state = APP_STATE_IDLE;
            app->transition_count++;
            return true;
        }

        if ((event->signal == EVENT_SIGNAL_APP_FAIL) || (event->signal == EVENT_SIGNAL_APP_TIMEOUT))
        {
            app->state = APP_STATE_ERROR;
            app->transition_count++;
            return true;
        }
        break;

        case APP_STATE_ERROR : if (event->signal == EVENT_SIGNAL_APP_RESET)
        {
            app->state = APP_STATE_IDLE;
            app->transition_count++;
            return true;
        }
        break;

        default:
        break;
    }

    app->unhandled_count++;
    return false;
}

const char *app_sm_state_name(app_state_t state)
{
    switch (state)
    {
        case APP_STATE_IDLE : return"IDLE";

        case APP_STATE_RUNNING : return"RUNNING";

        case APP_STATE_ERROR : return"ERROR";

        default:
        return"UNKNOWN";
    }
}
