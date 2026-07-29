#include <stdbool.h>
#include <stdlib.h>

bool *kidsWithCandies(int *candies, int candiesSize, int extraCandies, int *returnSize)
{
    bool *result = malloc((size_t)candiesSize * sizeof(*result));
    int greatest = 0;

    *returnSize = 0;

    if ((candiesSize > 0) && (result == NULL))
    {
        return NULL;
    }

    for (int index = 0; index < candiesSize; index++)
    {
        if (candies[index] > greatest)
        {
            greatest = candies[index];
        }
    }

    for (int index = 0; index < candiesSize; index++)
    {
        result[index] = (candies[index] + extraCandies) >= greatest;
    }

    *returnSize = candiesSize;
    return result;
}
