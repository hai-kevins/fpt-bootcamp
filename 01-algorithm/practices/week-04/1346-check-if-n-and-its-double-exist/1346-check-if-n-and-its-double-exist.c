#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>

static int compareIntegers(const void *left, const void *right)
{
    int leftValue = *(const int *)left;
    int rightValue = *(const int *)right;

    return (leftValue > rightValue) - (leftValue < rightValue);
}

static int lowerBound(const int *values, int valueCount, int target)
{
    int left = 0;
    int right = valueCount;

    while (left < right)
    {
        int middle = left + ((right - left) / 2);

        if (values[middle] < target)
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

bool checkIfExist(int *arr, int arrSize)
{
    qsort(arr, (size_t)arrSize, sizeof(*arr), compareIntegers);

    for (int index = 0; index < arrSize; index++)
    {
        long long doubled = (long long)arr[index] * 2LL;

        if ((doubled < INT_MIN) || (doubled > INT_MAX))
        {
            continue;
        }

        int candidate = lowerBound(arr, arrSize, (int)doubled);

        if ((candidate < arrSize) && (arr[candidate] == (int)doubled))
        {
            if (candidate != index)
            {
                return true;
            }

            if (((candidate + 1) < arrSize) &&
                (arr[candidate + 1] == (int)doubled))
            {
                return true;
            }
        }
    }

    return false;
}
