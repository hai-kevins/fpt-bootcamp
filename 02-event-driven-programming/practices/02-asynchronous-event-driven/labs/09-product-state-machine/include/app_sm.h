#ifndef LAB09_APP_SM_H
#define LAB09_APP_SM_H
typedef enum { APP_BOOT=0, APP_IDLE, APP_ACTIVE, APP_ERROR } app_state_t;
typedef enum
{
    APP_SYSTEM_START=0,
    APP_BUTTON_PRESSED,
    APP_SENSOR_READY,
    APP_ERROR_DETECTED,
    APP_RESET_REQUEST
} app_event_t;
typedef struct
{
    app_state_t state;
    unsigned transition_count;
} app_sm_t;
void app_sm_init(app_sm_t *sm);
void app_sm_dispatch(app_sm_t *sm, app_event_t event);
#endif
