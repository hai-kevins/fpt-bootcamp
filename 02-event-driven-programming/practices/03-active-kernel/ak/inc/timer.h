#ifndef AK_TIMER_H
#define AK_TIMER_H

#include <stdbool.h>
#include <stdint.h>

typedef uint8_t ak_timer_id_t;

#define AK_TIMER_ID_INVALID (0xFFU)

typedef struct
{
    uint8_t active_count;
    uint8_t active_max;
    uint32_t starts;
    uint32_t expirations;
    uint32_t post_failures;
} ak_timer_stats_t;

void ak_timer_system_init(void);

ak_timer_id_t ak_timer_start_one_shot(uint8_t source, uint8_t destination, uint16_t signal, uint32_t delay_ms);

ak_timer_id_t ak_timer_start_periodic(uint8_t source, uint8_t destination, uint16_t signal, uint32_t period_ms);

bool ak_timer_restart(ak_timer_id_t timer_id, uint32_t period_ms);
bool ak_timer_cancel(ak_timer_id_t timer_id);
void ak_timer_process(uint32_t now_ms);
ak_timer_stats_t ak_timer_stats(void);

#endif
