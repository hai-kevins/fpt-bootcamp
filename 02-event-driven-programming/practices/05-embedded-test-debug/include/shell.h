#ifndef SHELL_H
#define SHELL_H

#include "app_sm.h"
#include "event_pool.h"
#include "event_queue.h"
#include "event_trace.h"
#include "fault_injection.h"

#include <stddef.h>

typedef struct
{
    app_sm_t *app;
    event_queue_t *queue;
    event_pool_t *pool;
    event_trace_t *trace;
    fault_injection_t *faults;
} shell_context_t;

int shell_execute(shell_context_t *context, const char *line, char *output, size_t output_size);

#endif
