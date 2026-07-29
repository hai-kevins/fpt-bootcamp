#include <stddef.h>

/* LeetCode provides struct ListNode. */

struct ListNode *detectCycle(struct ListNode *head)
{
    struct ListNode *slow = head;
    struct ListNode *fast = head;

    do
    {
        if ((fast == NULL) || (fast->next == NULL))
        {
            return NULL;
        }

        slow = slow->next;
        fast = fast->next->next;
    }
    while (slow != fast);

    slow = head;

    while (slow != fast)
    {
        slow = slow->next;
        fast = fast->next;
    }

    return slow;
}
