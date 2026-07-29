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

void deleteNode(struct ListNode *node)
{
    struct ListNode *removedNode = node->next;

    node->val = removedNode->val;
    node->next = removedNode->next;
    free(removedNode);
}
