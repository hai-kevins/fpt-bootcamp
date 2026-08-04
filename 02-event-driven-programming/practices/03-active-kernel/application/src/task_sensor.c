#include "task_sensor.h"

#include "app_signals.h"
#include "app_tasks.h"
#include "message.h"
#include "task.h"

#include <stdint.h>

typedef struct
{
    uint16_t temperature;
    uint16_t voltage;
} sensor_data_t;

void task_sensor_init(void)
{
}

void task_sensor_handler(const ak_message_t *message)
{
    if (message->signal == APP_SIGNAL_SENSOR_SAMPLE)
    {
        const sensor_data_t data =
        {
            .temperature = 250U,
            .voltage = 3300U
        };
        ak_message_t *sample = ak_message_create_common(APP_TASK_SENSOR, APP_TASK_APPLICATION, APP_SIGNAL_SENSOR_DATA,
        &data, sizeof(data));
        if ((sample != 0) && !ak_task_post(sample))
        {
            ak_message_release(sample);
        }
    }
}
