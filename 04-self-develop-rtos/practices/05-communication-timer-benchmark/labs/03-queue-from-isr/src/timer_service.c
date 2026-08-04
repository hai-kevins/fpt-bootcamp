#include "timer_service.h"
#include "scheduler.h"
#include "software_timer.h"

static hr_semaphore_t *g_signal;

void hr_timer_service_bind(hr_semaphore_t *signal)
{
    g_signal = signal;
}

void hr_timer_service_task(void *argument)
{
    (void) argument;
    for (;;)
    {
        if ((g_signal != (hr_semaphore_t *)0) && (hr_semaphore_take(g_signal, HR_WAIT_FOREVER) == HR_WAIT_SUCCESS))
        {
            for (;;)
            {
                const uint32_t now = hr_scheduler_tick_now();
                hr_software_timer_t *timer = hr_software_timer_take_due(now);
                if (timer == (hr_software_timer_t *)0)
                {
                    break;
                }
                timer->callback(timer->argument);
                hr_software_timer_complete(timer, hr_scheduler_tick_now());
            }
        }
    }
}
