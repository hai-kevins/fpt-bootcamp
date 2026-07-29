#include <stddef.h>
#include <stdlib.h>

typedef struct
{
    int value;
    int frequency;
} FrequencyEntry;

static int compareIntegers(const void *left, const void *right)
{
    int leftValue = *(const int *)left;
    int rightValue = *(const int *)right;
    return (leftValue > rightValue) - (leftValue < rightValue);
}

static int compareFrequencyDescending(const void *left, const void *right)
{
    const FrequencyEntry *leftEntry = (const FrequencyEntry *)left;
    const FrequencyEntry *rightEntry = (const FrequencyEntry *)right;

    if (leftEntry->frequency != rightEntry->frequency)
    {
        return (rightEntry->frequency > leftEntry->frequency) -
               (rightEntry->frequency < leftEntry->frequency);
    }

    return (leftEntry->value > rightEntry->value) -
           (leftEntry->value < rightEntry->value);
}

int *topKFrequent(int *nums, int numsSize, int k, int *returnSize)
{
    int *sorted = NULL;
    FrequencyEntry *entries = NULL;
    int *result = NULL;
    int uniqueCount = 0;

    *returnSize = 0;

    if ((nums == NULL) || (numsSize <= 0) || (k <= 0))
    {
        return NULL;
    }

    sorted = (int *)malloc((size_t)numsSize * sizeof(int));
    entries = (FrequencyEntry *)malloc((size_t)numsSize * sizeof(FrequencyEntry));

    if ((sorted == NULL) || (entries == NULL))
    {
        free(sorted);
        free(entries);
        return NULL;
    }

    for (int index = 0; index < numsSize; index++)
    {
        sorted[index] = nums[index];
    }

    qsort(sorted, (size_t)numsSize, sizeof(int), compareIntegers);

    for (int index = 0; index < numsSize;)
    {
        int next = index + 1;

        while ((next < numsSize) && (sorted[next] == sorted[index]))
        {
            next++;
        }

        entries[uniqueCount].value = sorted[index];
        entries[uniqueCount].frequency = next - index;
        uniqueCount++;
        index = next;
    }

    qsort(entries,
          (size_t)uniqueCount,
          sizeof(FrequencyEntry),
          compareFrequencyDescending);

    if (k > uniqueCount)
    {
        k = uniqueCount;
    }

    result = (int *)malloc((size_t)k * sizeof(int));

    if (result != NULL)
    {
        for (int index = 0; index < k; index++)
        {
            result[index] = entries[index].value;
        }

        *returnSize = k;
    }

    free(sorted);
    free(entries);
    return result;
}
