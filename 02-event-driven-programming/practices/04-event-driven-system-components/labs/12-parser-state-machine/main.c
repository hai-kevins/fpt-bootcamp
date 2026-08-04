#include "parser.h"
#include "serializer.h"
#include <stdio.h>
int main(void)
{
    ed_frame_t f =
    {
        0
    },
    out;
    ed_parser_t p;
    uint8_t b[ED_FRAME_MAX_SIZE];
    size_t n = 0;
    f.signal = 3U;
    f.payload_length = 1U;
    f.payload[0] = 42U;
    ed_frame_encode(&f, b, sizeof(b), &n);
    ed_parser_init(&p);
    bool ready = false;
    ed_parser_feed(&p, 0x00U, &out);
    for (size_t i = 0; i < n; i++)
    {
        ready = ed_parser_feed(&p, b[i], &out) || ready;
    }
    (void) printf("ready=%d frames=%lu\n", ready ? 1 : 0, (unsigned long) p.frame_count);
    return (ready && out.payload[0] == 42U) ? 0 : 1;
}
