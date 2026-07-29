#include <stddef.h>
#include <stdlib.h>

int *nextGreaterElement(
    int *nums1,
    int nums1Size,
    int *nums2,
    int nums2Size,
    int *returnSize)
{
    *returnSize = nums1Size;

    if (nums1Size <= 0)
    {
        return NULL;
    }

    int *result = malloc((size_t)nums1Size * sizeof(*result));
    int *nextGreater = malloc((size_t)nums2Size * sizeof(*nextGreater));
    int *stack = malloc((size_t)nums2Size * sizeof(*stack));

    if ((result == NULL) || (nextGreater == NULL) || (stack == NULL))
    {
        free(result);
        free(nextGreater);
        free(stack);
        *returnSize = 0;
        return NULL;
    }

    int stackSize = 0;

    for (int index = nums2Size - 1; index >= 0; index--)
    {
        while ((stackSize > 0) && (stack[stackSize - 1] <= nums2[index]))
        {
            stackSize--;
        }

        nextGreater[index] = (stackSize > 0) ? stack[stackSize - 1] : -1;
        stack[stackSize++] = nums2[index];
    }

    for (int index1 = 0; index1 < nums1Size; index1++)
    {
        result[index1] = -1;

        for (int index2 = 0; index2 < nums2Size; index2++)
        {
            if (nums1[index1] == nums2[index2])
            {
                result[index1] = nextGreater[index2];
                break;
            }
        }
    }

    free(nextGreater);
    free(stack);
    return result;
}
