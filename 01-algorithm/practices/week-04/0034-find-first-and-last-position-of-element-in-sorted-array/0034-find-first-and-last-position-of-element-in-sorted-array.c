#include <stddef.h>
#include <stdlib.h>

static int lowerBound(const int *nums, int numsSize, int target)
{
    int left = 0;
    int right = numsSize;

    while (left < right)
    {
        int middle = left + ((right - left) / 2);

        if (nums[middle] < target)
        {
            left = middle + 1;
        }
        else
        {
            right = middle;
        }
    }

    return left;
}

static int upperBound(const int *nums, int numsSize, int target)
{
    int left = 0;
    int right = numsSize;

    while (left < right)
    {
        int middle = left + ((right - left) / 2);

        if (nums[middle] <= target)
        {
            left = middle + 1;
        }
        else
        {
            right = middle;
        }
    }

    return left;
}

int *searchRange(int *nums, int numsSize, int target, int *returnSize)
{
    int *range = malloc(2U * sizeof(*range));

    *returnSize = 2;

    if (range == NULL)
    {
        *returnSize = 0;
        return NULL;
    }

    int first = lowerBound(nums, numsSize, target);

    if ((first == numsSize) || (nums[first] != target))
    {
        range[0] = -1;
        range[1] = -1;
        return range;
    }

    range[0] = first;
    range[1] = upperBound(nums, numsSize, target) - 1;
    return range;
}
