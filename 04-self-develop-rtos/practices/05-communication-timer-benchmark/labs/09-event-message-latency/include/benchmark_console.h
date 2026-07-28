#ifndef HR_BENCHMARK_CONSOLE_H
#define HR_BENCHMARK_CONSOLE_H

#include "benchmark_stats.h"
#include "message_pool.h"
#include "queue.h"
#include "software_timer.h"

void hr_benchmark_console_bind(hr_queue_t *queue,
                               hr_message_pool_t *pool,
                               hr_software_timer_t *one_shot,
                               hr_software_timer_t *periodic,
                               hr_benchmark_stats_t *switch_stats,
                               hr_benchmark_stats_t *message_stats,
                               hr_benchmark_stats_t *timer_stats);
void hr_benchmark_console_print_help(void);
void hr_benchmark_console_process(uint8_t command);

#endif
