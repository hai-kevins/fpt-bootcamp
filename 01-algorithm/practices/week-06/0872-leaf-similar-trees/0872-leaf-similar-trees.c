#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

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

struct LeafSequence
{
    int *values;
    int size;
    int capacity;
    bool allocationFailed;
};

static void appendLeaf(struct LeafSequence *sequence, int value)
{
    if (sequence->allocationFailed)
    {
        return;
    }

    if (sequence->size == sequence->capacity)
    {
        int newCapacity = (sequence->capacity == 0)
            ? 16
            : sequence->capacity * 2;
        int *newValues = realloc(
            sequence->values,
            (size_t)newCapacity * sizeof(*newValues));

        if (newValues == NULL)
        {
            sequence->allocationFailed = true;
            return;
        }

        sequence->values = newValues;
        sequence->capacity = newCapacity;
    }

    sequence->values[sequence->size++] = value;
}

static void collectLeaves(
    struct TreeNode *node,
    struct LeafSequence *sequence)
{
    if ((node == NULL) || sequence->allocationFailed)
    {
        return;
    }

    if ((node->left == NULL) && (node->right == NULL))
    {
        appendLeaf(sequence, node->val);
        return;
    }

    collectLeaves(node->left, sequence);
    collectLeaves(node->right, sequence);
}

bool leafSimilar(struct TreeNode *root1, struct TreeNode *root2)
{
    struct LeafSequence first = {NULL, 0, 0, false};
    struct LeafSequence second = {NULL, 0, 0, false};

    collectLeaves(root1, &first);
    collectLeaves(root2, &second);

    bool areSimilar =
        !first.allocationFailed &&
        !second.allocationFailed &&
        (first.size == second.size);

    for (int index = 0; areSimilar && (index < first.size); index++)
    {
        if (first.values[index] != second.values[index])
        {
            areSimilar = false;
        }
    }

    free(first.values);
    free(second.values);
    return areSimilar;
}
