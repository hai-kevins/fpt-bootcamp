#ifndef ED_MAILBOX_H
#define ED_MAILBOX_H

#include "event.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define ED_MAILBOX_STORAGE_MAX (32U)

typedef enum
{
    ED_MAILBOX_DROP_NEWEST = 0,
    ED_MAILBOX_DROP_OLDEST
} ed_mailbox_overflow_policy_t;

typedef struct
{
    ed_event_t *storage[ED_MAILBOX_STORAGE_MAX];
    size_t capacity;
    size_t head;
    size_t tail;
    size_t count;
    size_t high_water_mark;
    uint32_t post_count;
    uint32_t get_count;
    uint32_t overflow_count;
    ed_mailbox_overflow_policy_t policy;
} ed_mailbox_t;

bool ed_mailbox_init(ed_mailbox_t *mailbox,
                     size_t capacity,
                     ed_mailbox_overflow_policy_t policy);
bool ed_mailbox_post(ed_mailbox_t *mailbox, ed_event_t *event);
ed_event_t *ed_mailbox_get(ed_mailbox_t *mailbox);
bool ed_mailbox_is_empty(const ed_mailbox_t *mailbox);
size_t ed_mailbox_count(const ed_mailbox_t *mailbox);

#endif
