#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/*
 * LeetCode provides these structures:
 *
 * struct ListNode
 * {
 *     int val;
 *     struct ListNode *next;
 * };
 *
 * struct TreeNode
 * {
 *     int val;
 *     struct TreeNode *left;
 *     struct TreeNode *right;
 * };
 */

static void destroyTree(struct TreeNode *root)
{
    if (root == NULL)
    {
        return;
    }

    destroyTree(root->left);
    destroyTree(root->right);
    free(root);
}

static struct TreeNode *buildBalancedTree(
    const int *values,
    int left,
    int right,
    bool *allocationSucceeded)
{
    if ((left > right) || !(*allocationSucceeded))
    {
        return NULL;
    }

    int middle = left + ((right - left) / 2);
    struct TreeNode *root = malloc(sizeof(*root));

    if (root == NULL)
    {
        *allocationSucceeded = false;
        return NULL;
    }

    root->val = values[middle];
    root->left = buildBalancedTree(
        values,
        left,
        middle - 1,
        allocationSucceeded);
    root->right = buildBalancedTree(
        values,
        middle + 1,
        right,
        allocationSucceeded);

    if (!(*allocationSucceeded))
    {
        destroyTree(root);
        return NULL;
    }

    return root;
}

struct TreeNode *sortedListToBST(struct ListNode *head)
{
    int nodeCount = 0;
    struct ListNode *current = head;

    while (current != NULL)
    {
        nodeCount++;
        current = current->next;
    }

    if (nodeCount == 0)
    {
        return NULL;
    }

    int *values = malloc((size_t)nodeCount * sizeof(*values));

    if (values == NULL)
    {
        return NULL;
    }

    current = head;

    for (int index = 0; index < nodeCount; index++)
    {
        values[index] = current->val;
        current = current->next;
    }

    bool allocationSucceeded = true;
    struct TreeNode *root = buildBalancedTree(
        values,
        0,
        nodeCount - 1,
        &allocationSucceeded);

    free(values);
    return root;
}
