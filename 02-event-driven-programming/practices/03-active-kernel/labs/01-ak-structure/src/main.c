#include <stdint.h>
#include <stdio.h>

typedef struct
{
    uint8_t id,
    priority,
    queue_depth;
} task_t;

typedef struct
{
    uint16_t signal;
    uint8_t kind,
    length,
    ref_count;
} message_t;

typedef struct
{
    uint8_t active,
    periodic;
    uint32_t deadline,
    period;
} timer_t;

int main(void)
{
    printf("task_t=%zu message_t=%zu timer_t=%zu\n", sizeof(task_t), sizeof(message_t), sizeof(timer_t));
    printf("priority: higher number runs first\n");
    printf("message kinds: pure common dynamic\n");
    printf("timer kinds: one-shot periodic\n");
    printf("FSM=function dispatch, TSM=transition table\n");
    return 0;
}
