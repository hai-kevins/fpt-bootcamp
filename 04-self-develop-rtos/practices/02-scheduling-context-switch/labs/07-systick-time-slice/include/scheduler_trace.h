#ifndef SCHEDULER_TRACE_H
#define SCHEDULER_TRACE_H

#include <stddef.h>
#include <stdint.h>

typedef enum
{
    RTOS_TRACE_KERNEL_START = 1,
    RTOS_TRACE_SWITCH,
    RTOS_TRACE_YIELD,
    RTOS_TRACE_DELAY,
    RTOS_TRACE_WAKE,
    RTOS_TRACE_TICK_ROTATE,
    RTOS_TRACE_ISR_WAKE
} rtos_trace_type_t;

typedef struct
{
    uint32_t tick;
    uint32_t sequence;
    rtos_trace_type_t type;
    uint8_t from_task;
    uint8_t to_task;
} rtos_trace_record_t;

void rtos_trace_reset(void);
void rtos_trace_record(rtos_trace_type_t type, uint8_t from_task, uint8_t to_task, uint32_t tick);
size_t rtos_trace_snapshot(rtos_trace_record_t *output, size_t capacity);

#endif
