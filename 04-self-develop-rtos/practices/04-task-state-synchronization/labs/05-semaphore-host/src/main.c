#include "model.h"
#include <stdio.h>

int main(void)
{
    sem_t s;
    task_t a =
    {
        "A",
        2,
        false,
        0
    },
    b =
    {
        "B",
        0,
        false,
        0
    };
    task_t *w = 0;
    sem_init(&s, 0, 1);
    sem_take(&s, &a, true);
    sem_take(&s, &b, true);
    while (sem_give(&s, &w) && w)
    {
        printf("wake %s\n", w->name);
    }
    return 0;
}
