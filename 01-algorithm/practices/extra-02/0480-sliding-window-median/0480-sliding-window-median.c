#include <stddef.h>
#include <stdlib.h>

static int compareIntegers(const void *left, const void *right)
{
    int leftValue = *(const int *)left;
    int rightValue = *(const int *)right;
    return (leftValue > rightValue) - (leftValue < rightValue);
}

static int lowerBound(const int *values, int size, int target)
{
    int left = 0;
    int right = size;

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

static void fenwickAdd(int *tree, int size, int index, int delta)
{
    for (int current = index; current <= size; current += current & -current)
    {
        tree[current] += delta;
    }
}

static int fenwickFindByOrder(const int *tree, int size, int order)
{
    int index = 0;
    int bit = 1;

    while ((bit << 1) <= size)
    {
        bit <<= 1;
    }

    for (; bit != 0; bit >>= 1)
    {
        int next = index + bit;

        if ((next <= size) && (tree[next] < order))
        {
            index = next;
            order -= tree[next];
        }
    }

    return index + 1;
}

double *medianSlidingWindow(int *nums,
                            int numsSize,
                            int k,
                            int *returnSize)
{
    int *sorted = NULL;
    int *unique = NULL;
    int *fenwick = NULL;
    int uniqueCount = 0;
    int outputCount = 0;
    double *result = NULL;

    *returnSize = 0;

    if ((nums == NULL) || (numsSize <= 0) || (k <= 0) || (k > numsSize))
    {
        return NULL;
    }

    sorted = (int *)malloc((size_t)numsSize * sizeof(int));
    unique = (int *)malloc((size_t)numsSize * sizeof(int));

    if ((sorted == NULL) || (unique == NULL))
    {
        free(sorted);
        free(unique);
        return NULL;
    }

    for (int index = 0; index < numsSize; index++)
    {
        sorted[index] = nums[index];
    }

    qsort(sorted, (size_t)numsSize, sizeof(int), compareIntegers);

    for (int index = 0; index < numsSize; index++)
    {
        if ((uniqueCount == 0) || (sorted[index] != unique[uniqueCount - 1]))
        {
            unique[uniqueCount++] = sorted[index];
        }
    }

    fenwick = (int *)calloc((size_t)(uniqueCount + 1), sizeof(int));
    outputCount = numsSize - k + 1;
    result = (double *)malloc((size_t)outputCount * sizeof(double));

    if ((fenwick == NULL) || (result == NULL))
    {
        free(sorted);
        free(unique);
        free(fenwick);
        free(result);
        return NULL;
    }

    for (int index = 0; index < k; index++)
    {
        int compressed = lowerBound(unique, uniqueCount, nums[index]) + 1;
        fenwickAdd(fenwick, uniqueCount, compressed, 1);
    }

    for (int start = 0; start < outputCount; start++)
    {
        int leftOrder = (k + 1) / 2;
        int rightOrder = (k + 2) / 2;
        int leftIndex = fenwickFindByOrder(fenwick, uniqueCount, leftOrder);
        int rightIndex = fenwickFindByOrder(fenwick, uniqueCount, rightOrder);
        int leftValue = unique[leftIndex - 1];
        int rightValue = unique[rightIndex - 1];

        result[start] = ((double)leftValue + (double)rightValue) / 2.0;

        if ((start + k) < numsSize)
        {
            int outgoing = lowerBound(unique, uniqueCount, nums[start]) + 1;
            int incoming = lowerBound(unique, uniqueCount, nums[start + k]) + 1;

            fenwickAdd(fenwick, uniqueCount, outgoing, -1);
            fenwickAdd(fenwick, uniqueCount, incoming, 1);
        }
    }

    *returnSize = outputCount;
    free(sorted);
    free(unique);
    free(fenwick);
    return result;
}
