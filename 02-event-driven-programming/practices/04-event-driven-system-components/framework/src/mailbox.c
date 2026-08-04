#include "mailbox.h"

#include <string.h>

bool ed_mailbox_init(ed_mailbox_t *mailbox, size_t capacity, ed_mailbox_overflow_policy_t policy)
{
    if ((mailbox == NULL) || (capacity == 0U) || (capacity > ED_MAILBOX_STORAGE_MAX))
    {
        return false;
    }

    (void) memset(mailbox, 0, sizeof (*mailbox));
    mailbox->capacity = capacity;
    mailbox->policy = policy;
    return true;
}

bool ed_mailbox_post(ed_mailbox_t *mailbox, ed_event_t *event)
{
    if ((mailbox == NULL) || (event == NULL) || (mailbox->capacity == 0U))
    {
        return false;
    }

    if (mailbox->count == mailbox->capacity)
    {
        mailbox->overflow_count++;

        if (mailbox->policy == ED_MAILBOX_DROP_NEWEST)
        {
            return false;
        }

        mailbox->tail = (mailbox->tail + 1U) % mailbox->capacity;
        mailbox->count--;
    }

    mailbox->storage[mailbox->head] = event;
    mailbox->head = (mailbox->head + 1U) % mailbox->capacity;
    mailbox->count++;
    mailbox->post_count++;

    if (mailbox->count > mailbox->high_water_mark)
    {
        mailbox->high_water_mark = mailbox->count;
    }

    return true;
}

ed_event_t *ed_mailbox_get(ed_mailbox_t *mailbox)
{
    ed_event_t *event;

    if ((mailbox == NULL) || (mailbox->count == 0U))
    {
        return NULL;
    }

    event = mailbox->storage[mailbox->tail];
    mailbox->storage[mailbox->tail] = NULL;
    mailbox->tail = (mailbox->tail + 1U) % mailbox->capacity;
    mailbox->count--;
    mailbox->get_count++;
    return event;
}

bool ed_mailbox_is_empty(const ed_mailbox_t *mailbox)
{
    return (mailbox == NULL) || (mailbox->count == 0U);
}

size_t ed_mailbox_count(const ed_mailbox_t *mailbox)
{
    return (mailbox == NULL) ? 0U : mailbox->count;
}
