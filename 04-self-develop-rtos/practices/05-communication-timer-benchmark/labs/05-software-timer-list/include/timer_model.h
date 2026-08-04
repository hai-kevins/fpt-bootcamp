#ifndef TIMER_MODEL_H
#define TIMER_MODEL_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct timer_model
{
    struct timer_model * next;
    uint32_t expiry,
    period,
    count;
    bool active;
} timer_model_t;

typedef struct
{
    timer_model_t *head;
    size_t count;
} timer_list_t;

void timer_list_init(timer_list_t *);
bool timer_start(timer_list_t *, timer_model_t *, uint32_t, uint32_t, uint32_t);
bool timer_stop(timer_list_t *, timer_model_t *);
timer_model_t *timer_take_due(timer_list_t *, uint32_t);
void timer_complete(timer_list_t *, timer_model_t *, uint32_t);
bool timer_validate(const timer_list_t *);
#endif
