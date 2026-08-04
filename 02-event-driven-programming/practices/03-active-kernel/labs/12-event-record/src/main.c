#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#define CAP 64U

typedef struct
{
    uint32_t ts;
    uint16_t sig;
} rec_t;

static rec_t r[CAP];
static uint32_t head, count, over;

static void write_rec(uint32_t ts, uint16_t sig)
{
    r[head] = (rec_t)
    {
        ts,
        sig
    };
    head = (head + 1U) % CAP;
    if (count < CAP)
    {
        count++;
    }
    else
    {
        over++;
    }
}

int main(void)
{
    for (uint32_t i = 0; i < 70U; i++)
    {
        write_rec(i, (uint16_t) i);
    }
    assert(count == 64U && over == 6U);
    uint32_t oldest = (head + CAP - count) % CAP;
    printf("oldest ts=%lu newest ts=%lu overwritten=%lu\n", (unsigned long) r[oldest].ts, (unsigned long) r[(head + CAP - 1U) % CAP].ts,
        (unsigned long) over);
    count = 0;
    head = 0;
    over = 0;
    assert(count == 0U);
    return 0;
}
