#ifndef ED_PARSER_H
#define ED_PARSER_H

#include "frame.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef enum
{
    ED_PARSER_WAIT_SOF = 0,
    ED_PARSER_READ_HEADER,
    ED_PARSER_READ_BODY
} ed_parser_state_t;

typedef struct
{
    ed_parser_state_t state;
    uint8_t buffer[ED_FRAME_MAX_SIZE];
    size_t index;
    size_t expected_length;
    uint32_t frame_count;
    uint32_t error_count;
} ed_parser_t;

void ed_parser_init(ed_parser_t *parser);
bool ed_parser_feed(ed_parser_t *parser, uint8_t byte, ed_frame_t *frame_ready);

#endif
