#include "app_signals.h"
#include "demo_ao.h"
#include "event_pool.h"
#include "pubsub.h"
#include "scheduler.h"

#include <stdio.h>
#include <stdint.h>

int main(void)
{
    ed_event_pool_t pool;
    ed_scheduler_t scheduler;
    ed_pubsub_t pubsub;
    demo_ao_t display;
    demo_ao_t diagnostic;
    ed_event_t prototype;
    const uint32_t sensor_value = 1234U;

    ed_event_pool_init(&pool);
    ed_scheduler_init(&scheduler, &pool);
    ed_pubsub_init(&pubsub, &pool);

    if (!demo_ao_init(&display, 1U, 2U, "display") ||
        !demo_ao_init(&diagnostic, 2U, 1U, "diagnostic") ||
        !ed_scheduler_register(&scheduler, &display.super) ||
        !ed_scheduler_register(&scheduler, &diagnostic.super) ||
        !ed_pubsub_subscribe(&pubsub, SENSOR_DATA_SIG, &display.super) ||
        !ed_pubsub_subscribe(&pubsub, SENSOR_DATA_SIG, &diagnostic.super))
    {
        return 1;
    }

    ed_event_init_static(&prototype, SENSOR_DATA_SIG, 10U, 0U);
    if (!ed_event_set_payload(&prototype,
                              &sensor_value,
                              sizeof(sensor_value)))
    {
        return 1;
    }

    const size_t delivered = ed_pubsub_publish(&pubsub, &prototype);
    const size_t dispatched =
        ed_scheduler_run_until_idle(&scheduler, 16U);

    (void)printf("Event-Driven components demo\n");
    (void)printf("delivered=%zu dispatched=%zu\n",
                 delivered,
                 dispatched);
    (void)printf("display handled=%lu value=%lu\n",
                 (unsigned long)display.handled,
                 (unsigned long)display.last_value);
    (void)printf("diagnostic handled=%lu value=%lu\n",
                 (unsigned long)diagnostic.handled,
                 (unsigned long)diagnostic.last_value);
    (void)printf("pool high-water=%zu used=%zu\n",
                 pool.high_water_mark,
                 pool.used_count);

    return ((delivered == 2U) &&
            (dispatched == 2U) &&
            (pool.used_count == 0U)) ? 0 : 1;
}
