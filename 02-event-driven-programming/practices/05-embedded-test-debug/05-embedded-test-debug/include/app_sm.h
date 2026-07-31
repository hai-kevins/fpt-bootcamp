#ifndef APP_SM_H
#define APP_SM_H

#include "event.h"

#include <stdbool.h>
#include <stdint.h>

typedef enum
{
    APP_STATE_IDLE = 0,
    APP_STATE_RUNNING,
    APP_STATE_ERROR
} app_state_t;

typedef struct
{
    app_state_t state;
    uint32_t transition_count;
    uint32_t unhandled_count;
    uint16_t last_signal;
} app_sm_t;

void app_sm_init(app_sm_t *app);
bool app_sm_dispatch(app_sm_t *app, const event_t *event);
const char *app_sm_state_name(app_state_t state);

#endif
