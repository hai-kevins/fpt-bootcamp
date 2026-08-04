#include "timer_model.h"
#include <stdio.h>

int main(void)
{
    timer_list_t l;
    timer_model_t a =
    {
        0
    },
    b =
    {
        0
    };
    timer_list_init(&l);
    timer_start(&l, &a, 0, 25, 0);
    timer_start(&l, &b, 0, 10, 10);
    for (unsigned now = 0; now <= 30; now++)
    {
        timer_model_t *t;
        while ((t = timer_take_due(&l, now)))
        {
            printf("expiry at %u\n", now);
            timer_complete(&l, t, now);
        }
    }
    return 0;
}
