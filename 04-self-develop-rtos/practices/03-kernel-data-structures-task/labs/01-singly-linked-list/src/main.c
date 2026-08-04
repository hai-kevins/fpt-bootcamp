#include "slist.h"
#include <stdio.h>

int main(void)
{
    slist_t list;
    slist_node_t a;
    slist_node_t b;
    slist_node_t c;

    slist_init(&list);
    slist_node_init(&a, 10);
    slist_node_init(&b, 20);
    slist_node_init(&c, 30);

    (void) slist_push_back(&list, &a);
    (void) slist_push_back(&list, &b);
    (void) slist_push_front(&list, &c);

    for (slist_node_t *node = list.head; node != (slist_node_t *)0; node = node->next)
    {
        printf("%d ", node->value);
    }

    printf("\ncount=%zu valid=%s\n", list.count, slist_validate(&list) ? "yes" : "no");

    return 0;
}
