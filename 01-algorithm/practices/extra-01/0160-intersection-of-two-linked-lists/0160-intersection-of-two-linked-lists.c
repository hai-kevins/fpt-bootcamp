#include <stddef.h>

/* LeetCode provides struct ListNode. */

struct ListNode *getIntersectionNode(
    struct ListNode *headA,
    struct ListNode *headB)
{
    struct ListNode *pointerA = headA;
    struct ListNode *pointerB = headB;

    while (pointerA != pointerB)
    {
        pointerA = (pointerA == NULL) ? headB : pointerA->next;
        pointerB = (pointerB == NULL) ? headA : pointerB->next;
    }

    return pointerA;
}
