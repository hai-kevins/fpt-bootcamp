#include <stdbool.h>
#include <stdlib.h>

static int compareIntegers(const void *left, const void *right)
{
    int leftValue = *(const int *)left;
    int rightValue = *(const int *)right;

    return (leftValue > rightValue) - (leftValue < rightValue);
}

bool containsDuplicate(int *nums, int numsSize)
{
    qsort(nums, (size_t)numsSize, sizeof(*nums), compareIntegers);

    for (int index = 1; index < numsSize; index++)
    {
        if (nums[index] == nums[index - 1])
        {
            return true;
        }
    }

    return false;
}
