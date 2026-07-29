#include <stdlib.h>

int *sortedSquares(int *nums, int numsSize, int *returnSize)
{
    *returnSize = numsSize;

    if (numsSize == 0)
    {
        return NULL;
    }

    int *result = malloc((size_t)numsSize * sizeof(*result));

    if (result == NULL)
    {
        *returnSize = 0;
        return NULL;
    }

    int left = 0;
    int right = numsSize - 1;

    for (int writeIndex = numsSize - 1; writeIndex >= 0; writeIndex--)
    {
        int leftSquare = nums[left] * nums[left];
        int rightSquare = nums[right] * nums[right];

        if (leftSquare > rightSquare)
        {
            result[writeIndex] = leftSquare;
            left++;
        }
        else
        {
            result[writeIndex] = rightSquare;
            right--;
        }
    }

    return result;
}
