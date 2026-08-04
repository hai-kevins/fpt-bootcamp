#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef struct
{
    uint32_t calls;
    uint16_t last_signal;
    uint32_t last_argument;
} event_spy_t;

static void spy_post(event_spy_t *spy, uint16_t signal, uint32_t argument)
{
    spy->calls++;
    spy->last_signal = signal;
    spy->last_argument = argument;
}

static void handler(event_spy_t *spy)
{
    spy_post(spy, 10U, 1U);
    spy_post(spy, 20U, 7U);
}

int main(void)
{
    event_spy_t spy =
    {
        0
    };
    handler(&spy);

    const bool pass = (spy.calls == 2U) && (spy.last_signal == 20U) && (spy.last_argument == 7U);

    (void) printf("calls=%lu last_signal=%u last_argument=%lu %s\n", (unsigned long) spy.calls, spy.last_signal,
        (unsigned long) spy.last_argument, pass ? "PASS" : "FAIL");

    return pass ? 0 : 1;
}
