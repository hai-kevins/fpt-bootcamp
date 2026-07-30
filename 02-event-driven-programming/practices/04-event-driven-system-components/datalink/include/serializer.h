#ifndef ED_SERIALIZER_H
#define ED_SERIALIZER_H

#include "frame.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

bool ed_frame_encode(const ed_frame_t *frame,
                     uint8_t *buffer,
                     size_t capacity,
                     size_t *encoded_length);
bool ed_frame_decode(const uint8_t *buffer,
                     size_t length,
                     ed_frame_t *frame);
bool ed_frame_from_event(const ed_event_t *event,
                         uint8_t source_node,
                         uint8_t destination_node,
                         ed_frame_t *frame);
bool ed_frame_to_event(const ed_frame_t *frame,
                       ed_event_t *event);

#endif
