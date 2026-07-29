#include <stddef.h>

/*
 * LeetCode provides this structure:
 *
 * struct ListNode
 * {
 *     int val;
 *     struct ListNode *next;
 * };
 */

struct ListNode *reverseList(struct ListNode *head)
{
    struct ListNode *previous = NULL;
    struct ListNode *current = head;

    while (current != NULL)
    {
        struct ListNode *nextNode = current->next;
        current->next = previous;
        previous = current;
        current = nextNode;
    }

    return previous;
}
