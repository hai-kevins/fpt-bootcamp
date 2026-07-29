#include "app_sm.h"
#include <stddef.h>
void app_sm_init(app_sm_t *sm)
{
    if (sm==NULL) return;
    sm->state=APP_BOOT;
    sm->transition_count=0U;
}
void app_sm_dispatch(app_sm_t *sm, app_event_t event)
{
    app_state_t next;
    if (sm==NULL) return;
    next=sm->state;
    switch (sm->state)
    {
        case APP_BOOT:
            if (event==APP_SYSTEM_START) next=APP_IDLE;
            break;
        case APP_IDLE:
            if (event==APP_BUTTON_PRESSED) next=APP_ACTIVE;
            else if (event==APP_ERROR_DETECTED) next=APP_ERROR;
            break;
        case APP_ACTIVE:
            if (event==APP_BUTTON_PRESSED) next=APP_IDLE;
            else if (event==APP_ERROR_DETECTED) next=APP_ERROR;
            else if (event==APP_SENSOR_READY) next=APP_ACTIVE;
            break;
        case APP_ERROR:
            if (event==APP_RESET_REQUEST) next=APP_IDLE;
            break;
        default:
            next=APP_ERROR;
            break;
    }
    if (next!=sm->state)
    {
        sm->state=next;
        sm->transition_count++;
    }
}
