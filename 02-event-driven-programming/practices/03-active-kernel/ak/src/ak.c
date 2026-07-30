#include "ak.h"

#include "ak_port.h"

void ak_init(void)
{
    ak_event_record_init();
    ak_message_system_init();
    ak_task_system_init();
    ak_timer_system_init();
    ak_fatal_boot();
}

bool ak_run_once(void)
{
    return ak_task_run_once();
}

uint32_t ak_run_until_idle(uint32_t maximum_steps)
{
    return ak_task_run_until_idle(maximum_steps);
}

void ak_process_time(uint32_t now_ms)
{
    ak_timer_process(now_ms);
}
