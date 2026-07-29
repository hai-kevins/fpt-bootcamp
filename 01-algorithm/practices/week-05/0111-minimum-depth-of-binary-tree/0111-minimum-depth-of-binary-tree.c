#include <stddef.h>

/*
 * LeetCode provides this structure:
 *
 * struct TreeNode
 * {
 *     int val;
 *     struct TreeNode *left;
 *     struct TreeNode *right;
 * };
 */

int minDepth(struct TreeNode *root)
{
    if (root == NULL)
    {
        return 0;
    }

    int leftDepth = minDepth(root->left);
    int rightDepth = minDepth(root->right);

    if (leftDepth == 0)
    {
        return rightDepth + 1;
    }

    if (rightDepth == 0)
    {
        return leftDepth + 1;
    }

    return ((leftDepth < rightDepth) ? leftDepth : rightDepth) + 1;
}
