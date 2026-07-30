#ifndef ED_DATALINK_H
#define ED_DATALINK_H

#include "frame.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef bool (*ed_link_send_bytes_t)(void *context,
                                     const uint8_t *data,
                                     size_t length);

typedef enum
{
    ED_LINK_UP = 0,
    ED_LINK_WAIT_ACK,
    ED_LINK_DOWN
} ed_link_state_t;

typedef struct
{
    ed_link_send_bytes_t send_bytes;
    void *send_context;
    ed_link_state_t state;
    uint32_t timeout_ticks;
    uint32_t elapsed_ticks;
    uint8_t retry_limit;
    uint8_t retry_count;
    uint16_t pending_sequence;
    uint8_t pending_frame[ED_FRAME_MAX_SIZE];
    size_t pending_length;
    uint32_t tx_count;
    uint32_t retry_total;
    uint32_t failure_count;
} ed_datalink_t;

bool ed_datalink_init(ed_datalink_t *link,
                      ed_link_send_bytes_t send_bytes,
                      void *send_context,
                      uint32_t timeout_ticks,
                      uint8_t retry_limit);
bool ed_datalink_send(ed_datalink_t *link,
                      const ed_frame_t *frame,
                      bool require_ack);
void ed_datalink_tick(ed_datalink_t *link,
                      uint32_t elapsed_ticks);
bool ed_datalink_ack(ed_datalink_t *link,
                     uint16_t sequence);

#endif
