#include "software_timer.h"
static bool g_active;
static uint32_t g_deadline_ms;
static bool time_reached(uint32_t now_ms, uint32_t deadline_ms)
{
    return (int32_t)(now_ms - deadline_ms) >= 0;
}
void software_timer_init(void)
{
    g_active = false;
    g_deadline_ms = 0UL;
}
void software_timer_start(uint32_t now_ms, uint32_t delay_ms)
{
    g_active = true;
    g_deadline_ms = now_ms + delay_ms;
}
bool software_timer_expired(uint32_t now_ms)
{
    if (g_active && time_reached(now_ms, g_deadline_ms))
    {
        g_active = false;
        return true;
    }
    return false;
}
