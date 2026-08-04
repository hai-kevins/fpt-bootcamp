#include "serializer.h"
#include "crc16.h"

#include <string.h>

static void write_u16_le(uint8_t *buffer, uint16_t value)
{
    buffer[0] = (uint8_t)(value & 0xFFU);
    buffer[1] = (uint8_t)(value >> 8U);
}

static uint16_t read_u16_le(const uint8_t *buffer)
{
    return (uint16_t)((uint16_t) buffer[0] | ((uint16_t) buffer[1] << 8U));
}

bool ed_frame_encode(const ed_frame_t *frame, uint8_t *buffer, size_t capacity, size_t *encoded_length)
{
    if ((frame == NULL) || (buffer == NULL) || (encoded_length == NULL) || (frame->payload_length > ED_EVENT_PAYLOAD_MAX))
    {
        return false;
    }

    const size_t total = ED_FRAME_HEADER_LEN + frame->payload_length + ED_FRAME_CRC_LEN;
    if (capacity < total)
    {
        return false;
    }

    buffer[0] = ED_FRAME_SOF;
    buffer[1] = ED_FRAME_VERSION;
    buffer[2] = frame->source_node;
    buffer[3] = frame->destination_node;
    buffer[4] = frame->source_object;
    buffer[5] = frame->destination_object;
    write_u16_le(&buffer[6], frame->signal);
    write_u16_le(&buffer[8], frame->sequence);
    buffer[10] = frame->payload_length;

    if (frame->payload_length > 0U)
    {
        (void) memcpy(&buffer[ED_FRAME_HEADER_LEN], frame->payload, frame->payload_length);
    }

    const uint16_t crc = ed_crc16_ccitt(buffer, total - ED_FRAME_CRC_LEN);
    write_u16_le(&buffer[total - ED_FRAME_CRC_LEN], crc);
    *encoded_length = total;
    return true;
}

bool ed_frame_decode(const uint8_t *buffer, size_t length, ed_frame_t *frame)
{
    if ((buffer == NULL) || (frame == NULL) || (length < (ED_FRAME_HEADER_LEN + ED_FRAME_CRC_LEN)) || (buffer[0] != ED_FRAME_SOF)
        || (buffer[1] != ED_FRAME_VERSION))
    {
        return false;
    }

    const uint8_t payload_length = buffer[10];
    const size_t expected = ED_FRAME_HEADER_LEN + payload_length + ED_FRAME_CRC_LEN;
    if ((payload_length > ED_EVENT_PAYLOAD_MAX) || (length != expected))
    {
        return false;
    }

    const uint16_t received_crc = read_u16_le(&buffer[length - ED_FRAME_CRC_LEN]);
    const uint16_t calculated_crc = ed_crc16_ccitt(buffer, length - ED_FRAME_CRC_LEN);
    if (received_crc != calculated_crc)
    {
        return false;
    }

    (void) memset(frame, 0, sizeof (*frame));
    frame->source_node = buffer[2];
    frame->destination_node = buffer[3];
    frame->source_object = buffer[4];
    frame->destination_object = buffer[5];
    frame->signal = read_u16_le(&buffer[6]);
    frame->sequence = read_u16_le(&buffer[8]);
    frame->payload_length = payload_length;
    if (payload_length > 0U)
    {
        (void) memcpy(frame->payload, &buffer[ED_FRAME_HEADER_LEN], payload_length);
    }
    return true;
}

bool ed_frame_from_event(const ed_event_t *event, uint8_t source_node, uint8_t destination_node, ed_frame_t *frame)
{
    if ((event == NULL) || (frame == NULL))
    {
        return false;
    }

    (void) memset(frame, 0, sizeof (*frame));
    frame->source_node = source_node;
    frame->destination_node = destination_node;
    frame->source_object = event->source;
    frame->destination_object = event->destination;
    frame->signal = event->signal;
    frame->sequence = event->sequence;
    frame->payload_length = event->payload_length;
    if (event->payload_length > 0U)
    {
        (void) memcpy(frame->payload, event->payload, event->payload_length);
    }
    return true;
}

bool ed_frame_to_event(const ed_frame_t *frame, ed_event_t *event)
{
    if ((frame == NULL) || (event == NULL))
    {
        return false;
    }

    ed_event_init_static(event, frame->signal, frame->source_object, frame->destination_object);
    event->flags |= ED_EVENT_FLAG_REMOTE;
    event->sequence = frame->sequence;
    return ed_event_set_payload(event, frame->payload, frame->payload_length);
}
