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

static int calculateDepth(struct TreeNode *node, int *maximumDiameter)
{
    if (node == NULL)
    {
        return 0;
    }

    int leftDepth = calculateDepth(node->left, maximumDiameter);
    int rightDepth = calculateDepth(node->right, maximumDiameter);
    int diameterThroughNode = leftDepth + rightDepth;

    if (diameterThroughNode > *maximumDiameter)
    {
        *maximumDiameter = diameterThroughNode;
    }

    return ((leftDepth > rightDepth) ? leftDepth : rightDepth) + 1;
}

int diameterOfBinaryTree(struct TreeNode *root)
{
    int maximumDiameter = 0;

    (void)calculateDepth(root, &maximumDiameter);
    return maximumDiameter;
}
