#ifndef AK_TASK_H
#define AK_TASK_H

#include <stdbool.h>
#include <stdint.h>

#include "ak_config.h"
#include "message.h"

typedef void (*ak_task_handler_t)(const ak_message_t *message);

typedef struct
{
    uint8_t id;
    uint8_t priority;
    const char *name;
    uint8_t queue_depth;
    uint8_t queue_depth_max;
    uint32_t posted;
    uint32_t dispatched;
    uint32_t dropped;
    uint32_t handler_time_max;
} ak_task_stats_t;

void ak_task_system_init(void);

bool ak_task_register(
    uint8_t task_id,
    uint8_t priority,
    const char *name,
    ak_task_handler_t handler
);

/*
 * On success, queue/scheduler owns the message.
 * On failure, the caller still owns it and must release it.
 */
bool ak_task_post(ak_message_t *message);
bool ak_task_post_from_isr(ak_message_t *message);

bool ak_task_run_once(void);
uint32_t ak_task_run_until_idle(uint32_t maximum_steps);

bool ak_task_get_stats(uint8_t task_id, ak_task_stats_t *stats);
uint8_t ak_task_current_id(void);
uint32_t ak_task_interrupt_nesting(void);
void ak_task_entry_interrupt(void);
void ak_task_exit_interrupt(void);

#endif
