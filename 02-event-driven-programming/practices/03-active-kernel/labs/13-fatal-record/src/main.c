#include <assert.h>
#include <stdint.h>
#include <stdio.h>
typedef struct
{
    uint32_t magic,
    code,
    ts,
    restart,
    checksum;
    uint16_t sig;
    uint8_t task;
} fatal_t;
static fatal_t r;
static uint32_t sum(void)
{
    return r.magic ^ r.code ^ r.ts ^ r.restart ^ r.sig ^ r.task ^ 0x5A5AA5A5U;
}

static void capture(uint32_t code, uint8_t task, uint16_t sig, uint32_t ts)
{
    r.magic = 0x414B4654U;
    r.code = code;
    r.task = task;
    r.sig = sig;
    r.ts = ts;
    r.checksum = sum();
}

static int valid(void)
{
    return r.magic == 0x414B4654U && r.checksum == sum();
}

static void boot(void)
{
    if (valid())
    {
        r.restart++;
        r.checksum = sum();
    }
}

int main(void)
{
    capture(1001U, 4U, 10U, 1234U);
    assert(valid());
    boot();
    assert(r.restart == 1U && valid());
    printf("code=%lu task=%u timestamp=%lu restart=%lu\n", (unsigned long) r.code, r.task, (unsigned long) r.ts,
        (unsigned long) r.restart);
    return 0;
}
