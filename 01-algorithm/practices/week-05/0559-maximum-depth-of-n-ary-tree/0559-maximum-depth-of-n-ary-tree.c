#include <stddef.h>

/*
 * LeetCode provides this structure:
 *
 * struct Node
 * {
 *     int val;
 *     int numChildren;
 *     struct Node **children;
 * };
 */

int maxDepth(struct Node *root)
{
    if (root == NULL)
    {
        return 0;
    }

    int deepestChild = 0;

    for (int childIndex = 0; childIndex < root->numChildren; childIndex++)
    {
        int childDepth = maxDepth(root->children[childIndex]);

        if (childDepth > deepestChild)
        {
            deepestChild = childDepth;
        }
    }

    return deepestChild + 1;
}
