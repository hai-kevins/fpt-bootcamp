#ifndef ED_FRAME_H
#define ED_FRAME_H

#include "event.h"

#include <stddef.h>
#include <stdint.h>

#define ED_FRAME_SOF        (0xA5U)
#define ED_FRAME_VERSION    (1U)
#define ED_FRAME_HEADER_LEN (11U)
#define ED_FRAME_CRC_LEN    (2U)
#define ED_FRAME_MAX_SIZE   (ED_FRAME_HEADER_LEN + ED_EVENT_PAYLOAD_MAX + ED_FRAME_CRC_LEN)

typedef struct
{
    uint8_t source_node;
    uint8_t destination_node;
    uint8_t source_object;
    uint8_t destination_object;
    ed_signal_t signal;
    uint16_t sequence;
    uint8_t payload_length;
    uint8_t payload[ED_EVENT_PAYLOAD_MAX];
} ed_frame_t;

#endif
