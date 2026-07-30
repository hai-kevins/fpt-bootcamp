#include "parser.h"
#include "serializer.h"

#include <string.h>

static void parser_reset_state(ed_parser_t *parser)
{
    parser->state = ED_PARSER_WAIT_SOF;
    parser->index = 0U;
    parser->expected_length = 0U;
}

void ed_parser_init(ed_parser_t *parser)
{
    if (parser != NULL)
    {
        (void)memset(parser, 0, sizeof(*parser));
        parser_reset_state(parser);
    }
}

bool ed_parser_feed(ed_parser_t *parser,
                    uint8_t byte,
                    ed_frame_t *frame_ready)
{
    if ((parser == NULL) || (frame_ready == NULL))
    {
        return false;
    }

    if (parser->state == ED_PARSER_WAIT_SOF)
    {
        if (byte == ED_FRAME_SOF)
        {
            parser->buffer[0] = byte;
            parser->index = 1U;
            parser->state = ED_PARSER_READ_HEADER;
        }
        return false;
    }

    if (parser->index >= ED_FRAME_MAX_SIZE)
    {
        parser->error_count++;
        parser_reset_state(parser);
        return false;
    }

    parser->buffer[parser->index] = byte;
    parser->index++;

    if ((parser->state == ED_PARSER_READ_HEADER) &&
        (parser->index == ED_FRAME_HEADER_LEN))
    {
        const uint8_t length = parser->buffer[10];
        if (length > ED_EVENT_PAYLOAD_MAX)
        {
            parser->error_count++;
            parser_reset_state(parser);
            return false;
        }
        parser->expected_length = ED_FRAME_HEADER_LEN +
                                  length + ED_FRAME_CRC_LEN;
        parser->state = ED_PARSER_READ_BODY;
    }

    if ((parser->state == ED_PARSER_READ_BODY) &&
        (parser->index == parser->expected_length))
    {
        const bool valid = ed_frame_decode(parser->buffer,
                                           parser->expected_length,
                                           frame_ready);
        if (valid)
        {
            parser->frame_count++;
        }
        else
        {
            parser->error_count++;
        }
        parser_reset_state(parser);
        return valid;
    }

    return false;
}
