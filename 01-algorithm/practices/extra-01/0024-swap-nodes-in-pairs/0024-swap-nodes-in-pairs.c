#include <stddef.h>

/* LeetCode provides struct ListNode. */

struct ListNode *swapPairs(struct ListNode *head)
{
    struct ListNode sentinel = {0, head};
    struct ListNode *previous = &sentinel;

    while ((previous->next != NULL) && (previous->next->next != NULL))
    {
        struct ListNode *first = previous->next;
        struct ListNode *second = first->next;

        first->next = second->next;
        second->next = first;
        previous->next = second;
        previous = first;
    }

    return sentinel.next;
}
