#ifndef POLICY_H
#define POLICY_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define POLICY_PRIORITIES (4U)

typedef struct policy_task
{
    struct policy_task * next;
    uint8_t id;
    uint8_t priority;
    bool queued;
} policy_task_t;

typedef struct
{
    policy_task_t *head[POLICY_PRIORITIES];
    policy_task_t *tail[POLICY_PRIORITIES];
    size_t count[POLICY_PRIORITIES];
    uint32_t bitmap;
} policy_scheduler_t;

void policy_init(policy_scheduler_t *scheduler);
bool policy_add(policy_scheduler_t *scheduler, policy_task_t *task);
policy_task_t *policy_select(const policy_scheduler_t *scheduler);
bool policy_rotate(policy_scheduler_t *scheduler, uint8_t priority);
bool policy_remove(policy_scheduler_t *scheduler, policy_task_t *task);

#endif
