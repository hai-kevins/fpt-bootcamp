#include <stddef.h>
#include <stdlib.h>

int **intervalIntersection(
    int **firstList,
    int firstListSize,
    int *firstListColSize,
    int **secondList,
    int secondListSize,
    int *secondListColSize,
    int *returnSize,
    int **returnColumnSizes)
{
    (void)firstListColSize;
    (void)secondListColSize;

    int maximumIntersections = firstListSize + secondListSize;
    int **result = malloc((size_t)maximumIntersections * sizeof(*result));
    int *columnSizes = malloc((size_t)maximumIntersections * sizeof(*columnSizes));

    *returnSize = 0;
    *returnColumnSizes = NULL;

    if (((result == NULL) || (columnSizes == NULL)) && (maximumIntersections > 0))
    {
        free(result);
        free(columnSizes);
        return NULL;
    }

    int firstIndex = 0;
    int secondIndex = 0;
    int resultSize = 0;

    while ((firstIndex < firstListSize) && (secondIndex < secondListSize))
    {
        int start = (firstList[firstIndex][0] > secondList[secondIndex][0])
            ? firstList[firstIndex][0]
            : secondList[secondIndex][0];
        int end = (firstList[firstIndex][1] < secondList[secondIndex][1])
            ? firstList[firstIndex][1]
            : secondList[secondIndex][1];

        if (start <= end)
        {
            result[resultSize] = malloc(2U * sizeof(int));

            if (result[resultSize] == NULL)
            {
                for (int index = 0; index < resultSize; index++)
                {
                    free(result[index]);
                }

                free(result);
                free(columnSizes);
                return NULL;
            }

            result[resultSize][0] = start;
            result[resultSize][1] = end;
            columnSizes[resultSize] = 2;
            resultSize++;
        }

        if (firstList[firstIndex][1] < secondList[secondIndex][1])
        {
            firstIndex++;
        }
        else
        {
            secondIndex++;
        }
    }

    *returnSize = resultSize;
    *returnColumnSizes = columnSizes;
    return result;
}
