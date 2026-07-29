#include <stdlib.h>

static int compareIntegers(const void *left, const void *right)
{
    int leftValue = *(const int *)left;
    int rightValue = *(const int *)right;

    return (leftValue > rightValue) - (leftValue < rightValue);
}

int arrayPairSum(int *nums, int numsSize)
{
    int sum = 0;

    qsort(nums, (size_t)numsSize, sizeof(*nums), compareIntegers);

    for (int index = 0; index < numsSize; index += 2)
    {
        sum += nums[index];
    }

    return sum;
}
