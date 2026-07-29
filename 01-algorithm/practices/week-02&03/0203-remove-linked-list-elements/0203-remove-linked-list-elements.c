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

struct ListNode *removeElements(struct ListNode *head, int val)
{
    struct ListNode **link = &head;

    while (*link != NULL)
    {
        if ((*link)->val == val)
        {
            struct ListNode *removedNode = *link;
            *link = removedNode->next;
            free(removedNode);
        }
        else
        {
            link = &(*link)->next;
        }
    }

    return head;
}
