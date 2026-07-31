#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef struct
{
    uint32_t deadline;
    uint32_t period;
    uint32_t expiries;
    bool active;
    bool periodic;
} timer_t;

static void start(
    timer_t *timer,
    uint32_t now,
    uint32_t delay,
    bool periodic
)
{
    timer->deadline = now + delay;
    timer->period = delay;
    timer->active = true;
    timer->periodic = periodic;
}

static void process(timer_t *timer, uint32_t now)
{
    while (timer->active && (now >= timer->deadline))
    {
        timer->expiries++;

        if (timer->periodic)
        {
            timer->deadline += timer->period;
        }
        else
        {
            timer->active = false;
        }
    }
}

int main(void)
{
    uint32_t now = 0U;
    timer_t one_shot = {0};
    timer_t periodic = {0};

    start(&one_shot, now, 1000U, false);
    start(&periodic, now, 1000U, true);

    now = 3000U;
    process(&one_shot, now);
    process(&periodic, now);

    const bool pass =
        (one_shot.expiries == 1U) &&
        (periodic.expiries == 3U);

    (void)printf(
        "one_shot=%lu periodic=%lu now=%lu %s\n",
        (unsigned long)one_shot.expiries,
        (unsigned long)periodic.expiries,
        (unsigned long)now,
        pass ? "PASS" : "FAIL"
    );

    return pass ? 0 : 1;
}
