#ifndef HR_SOFTWARE_TIMER_H
#define HR_SOFTWARE_TIMER_H

#include "list.h"
#include "semaphore.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef void (*hr_timer_callback_t)(void *argument);

typedef enum
{
    HR_TIMER_INACTIVE = 0,
    HR_TIMER_ACTIVE,
    HR_TIMER_CALLBACK_PENDING
} hr_timer_state_t;

typedef struct hr_software_timer
{
    hr_list_node_t node;
    const char *name;
    uint32_t expiry_tick;
    uint32_t period_ticks;
    uint32_t expiry_count;
    uint32_t missed_period_count;
    hr_timer_callback_t callback;
    void *argument;
    hr_timer_state_t state;
    bool periodic;
} hr_software_timer_t;

void hr_software_timer_system_init(hr_semaphore_t *service_signal);
bool hr_software_timer_init(hr_software_timer_t *timer, const char *name, hr_timer_callback_t callback, void *argument);
bool hr_software_timer_start(hr_software_timer_t *timer, uint32_t delay_ticks, uint32_t period_ticks);
bool hr_software_timer_stop(hr_software_timer_t *timer);
bool hr_software_timer_restart(hr_software_timer_t *timer, uint32_t delay_ticks);
void hr_software_timer_on_tick_from_isr(uint32_t now);
hr_software_timer_t *hr_software_timer_take_due(uint32_t now);
void hr_software_timer_complete(hr_software_timer_t *timer, uint32_t now);
size_t hr_software_timer_active_count(void);
bool hr_software_timer_validate(void);
const hr_list_t *hr_software_timer_active_list(void);

#endif
