#ifndef APP_H
#define APP_H

#include "event.h"

#include <stdint.h>

typedef enum
{
    APP_STATE_BOOT = 0,
    APP_STATE_IDLE,
    APP_STATE_ACTIVE,
    APP_STATE_ERROR
} app_state_t;

void app_init(void);
void app_event_handler(const event_t *event);
app_state_t app_get_state(void);
const char *app_state_name(app_state_t state);

#endif
