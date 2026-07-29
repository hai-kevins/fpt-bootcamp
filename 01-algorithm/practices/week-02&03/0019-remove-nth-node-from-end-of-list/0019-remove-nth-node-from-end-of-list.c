#include <stdlib.h>

/*
 * LeetCode provides this structure:
 *
 * struct ListNode
 * {
 *     int val;
 *     struct ListNode *next;
 * };
 */

struct ListNode *removeNthFromEnd(struct ListNode *head, int n)
{
    struct ListNode dummy;
    struct ListNode *fast = &dummy;
    struct ListNode *slow = &dummy;

    dummy.val = 0;
    dummy.next = head;

    for (int step = 0; step <= n; step++)
    {
        fast = fast->next;
    }

    while (fast != NULL)
    {
        fast = fast->next;
        slow = slow->next;
    }

    struct ListNode *removedNode = slow->next;
    slow->next = removedNode->next;
    free(removedNode);
    return dummy.next;
}
