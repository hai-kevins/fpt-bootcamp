#include "app_sm.h"
#include <stdio.h>

int main(void)
{
    app_sm_t sm;
    const app_event_t sequence[] =
    {
        APP_SYSTEM_START,
        APP_BUTTON_PRESSED,
        APP_SENSOR_READY,
        APP_ERROR_DETECTED,
        APP_RESET_REQUEST
    };
    app_sm_init(&sm);
    for (size_t i = 0U; i < sizeof(sequence) / sizeof(sequence[0]); i++)
    {
        app_sm_dispatch(&sm, sequence[i]);
        printf("event=%d state=%d transitions=%u\n", (int) sequence[i], (int) sm.state, sm.transition_count);
    }
    return (sm.state == APP_IDLE) ? 0 : 1;
}
