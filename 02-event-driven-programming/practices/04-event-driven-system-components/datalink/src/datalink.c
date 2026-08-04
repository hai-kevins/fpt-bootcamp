#include "datalink.h"
#include "serializer.h"

#include <string.h>

bool ed_datalink_init(ed_datalink_t *link, ed_link_send_bytes_t send_bytes, void *send_context, uint32_t timeout_ticks,
    uint8_t retry_limit)
{
    if ((link == NULL) || (send_bytes == NULL) || (timeout_ticks == 0U))
    {
        return false;
    }

    (void) memset(link, 0, sizeof (*link));
    link->send_bytes = send_bytes;
    link->send_context = send_context;
    link->timeout_ticks = timeout_ticks;
    link->retry_limit = retry_limit;
    link->state = ED_LINK_UP;
    return true;
}

bool ed_datalink_send(ed_datalink_t *link, const ed_frame_t *frame, bool require_ack)
{
    size_t length = 0U;

    if ((link == NULL) || (frame == NULL) || (link->state == ED_LINK_DOWN) || (require_ack && (link->state == ED_LINK_WAIT_ACK))
        || !ed_frame_encode(frame, link->pending_frame, sizeof(link->pending_frame), &length) || !link->send_bytes(link->send_context,
        link->pending_frame, length))
    {
        return false;
    }

    link->tx_count++;
    if (require_ack)
    {
        link->state = ED_LINK_WAIT_ACK;
        link->pending_sequence = frame->sequence;
        link->pending_length = length;
        link->elapsed_ticks = 0U;
        link->retry_count = 0U;
    }
    else
    {
        link->state = ED_LINK_UP;
        link->pending_length = 0U;
    }
    return true;
}

void ed_datalink_tick(ed_datalink_t *link, uint32_t elapsed_ticks)
{
    if ((link == NULL) || (link->state != ED_LINK_WAIT_ACK))
    {
        return;
    }

    link->elapsed_ticks += elapsed_ticks;
    if (link->elapsed_ticks < link->timeout_ticks)
    {
        return;
    }

    link->elapsed_ticks = 0U;
    if (link->retry_count >= link->retry_limit)
    {
        link->state = ED_LINK_DOWN;
        link->failure_count++;
        return;
    }

    if (link->send_bytes(link->send_context, link->pending_frame, link->pending_length))
    {
        link->retry_count++;
        link->retry_total++;
        link->tx_count++;
    }
}

bool ed_datalink_ack(ed_datalink_t *link, uint16_t sequence)
{
    if ((link == NULL) || (link->state != ED_LINK_WAIT_ACK) || (link->pending_sequence != sequence))
    {
        return false;
    }

    link->state = ED_LINK_UP;
    link->pending_length = 0U;
    link->elapsed_ticks = 0U;
    link->retry_count = 0U;
    return true;
}
