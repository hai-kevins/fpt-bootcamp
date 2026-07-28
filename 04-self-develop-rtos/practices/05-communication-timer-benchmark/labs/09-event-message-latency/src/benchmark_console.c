#include "benchmark_console.h"
#include "benchmark_clock.h"
#include "critical_section.h"
#include "scheduler.h"
#include "task_registry.h"
#include "uart.h"
#include <stdint.h>

static hr_queue_t *g_queue;
static hr_message_pool_t *g_pool;
static hr_software_timer_t *g_one_shot;
static hr_software_timer_t *g_periodic;
static hr_benchmark_stats_t *g_switch_stats;
static hr_benchmark_stats_t *g_message_stats;
static hr_benchmark_stats_t *g_timer_stats;

static void newline(void) { uart1_write_string("\r\n"); }
static void print_stat(const char *name, const hr_benchmark_stats_t *stats)
{
    uart1_write_string(name);
    uart1_write_string(" samples="); uart1_write_u32(stats->count);
    uart1_write_string(" min="); uart1_write_u32(stats->minimum);
    uart1_write_string(" max="); uart1_write_u32(stats->maximum);
    uart1_write_string(" avg="); uart1_write_u32(hr_benchmark_stats_average(stats));
    uart1_write_string(" lost="); uart1_write_u32(stats->lost_samples);
    newline();
}

void hr_benchmark_console_bind(hr_queue_t *queue,
                               hr_message_pool_t *pool,
                               hr_software_timer_t *one_shot,
                               hr_software_timer_t *periodic,
                               hr_benchmark_stats_t *switch_stats,
                               hr_benchmark_stats_t *message_stats,
                               hr_benchmark_stats_t *timer_stats)
{
    g_queue = queue;
    g_pool = pool;
    g_one_shot = one_shot;
    g_periodic = periodic;
    g_switch_stats = switch_stats;
    g_message_stats = message_stats;
    g_timer_stats = timer_stats;
}

void hr_benchmark_console_print_help(void)
{
    uart1_write_string("h help | q queue | p pool | t timers | s stats | v validate\r\n");
}

void hr_benchmark_console_process(uint8_t command)
{
    if (command == (uint8_t)'h') { hr_benchmark_console_print_help(); return; }
    if (command == (uint8_t)'q')
    {
        uart1_write_string("queue count="); uart1_write_u32((uint32_t)g_queue->count);
        uart1_write_string(" capacity="); uart1_write_u32((uint32_t)g_queue->capacity);
        uart1_write_string(" send="); uart1_write_u32(g_queue->send_count);
        uart1_write_string(" recv="); uart1_write_u32(g_queue->receive_count); newline();
        return;
    }
    if (command == (uint8_t)'p')
    {
        uart1_write_string("pool free="); uart1_write_u32((uint32_t)g_pool->free_count);
        uart1_write_string(" blocks="); uart1_write_u32((uint32_t)g_pool->block_count);
        uart1_write_string(" failures="); uart1_write_u32(g_pool->failure_count); newline();
        return;
    }
    if (command == (uint8_t)'t')
    {
        uart1_write_string("active timers="); uart1_write_u32((uint32_t)hr_software_timer_active_count());
        uart1_write_string(" one-shot state="); uart1_write_u32((uint32_t)g_one_shot->state);
        uart1_write_string(" periodic expiries="); uart1_write_u32(g_periodic->expiry_count); newline();
        return;
    }
    if (command == (uint8_t)'s')
    {
        print_stat("switch", g_switch_stats);
        print_stat("message", g_message_stats);
        print_stat("timer", g_timer_stats);
        uart1_write_string("timestamp overhead=");
        uart1_write_u32(hr_benchmark_timestamp_overhead()); newline();
        return;
    }
    if (command == (uint8_t)'v')
    {
        uart1_write_string("scheduler="); uart1_write_string(hr_scheduler_validate() ? "PASS" : "FAIL"); newline();
        uart1_write_string("queue="); uart1_write_string(hr_queue_validate(g_queue) ? "PASS" : "FAIL"); newline();
        uart1_write_string("pool="); uart1_write_string(hr_message_pool_validate(g_pool) ? "PASS" : "FAIL"); newline();
        uart1_write_string("timers="); uart1_write_string(hr_software_timer_validate() ? "PASS" : "FAIL"); newline();
        uart1_write_string("stats=");
        uart1_write_string((hr_benchmark_stats_validate(g_switch_stats) &&
                            hr_benchmark_stats_validate(g_message_stats) &&
                            hr_benchmark_stats_validate(g_timer_stats)) ? "PASS" : "FAIL"); newline();
        return;
    }
    uart1_write_string("unknown command\r\n");
}
