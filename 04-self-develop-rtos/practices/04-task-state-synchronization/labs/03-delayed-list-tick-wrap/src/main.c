#include "model.h"
#include <stdio.h>

int main(void)
{
    delayed_t d;
    task_t a =
    {
        "A",
        105,
        false
    },
    b =
    {
        "B",
        101,
        false
    },
    c =
    {
        "C",
        110,
        false
    };
    delayed_init(&d);
    delayed_insert(&d, &a);
    delayed_insert(&d, &b);
    delayed_insert(&d, &c);
    while (d.count)
    {
        task_t *t = delayed_take_due(&d, d.items[0]->wake);
        printf("wake %s at %u\n", t->name, t->wake);
    }
    return 0;
}
