#include <stddef.h>
#include <stdlib.h>

/*
 * LeetCode provides this interface:
 *
 * struct CustomFunction
 * {
 *     int (*f)(int x, int y);
 * };
 */

static void releaseSolutions(int **solutions, int solutionCount)
{
    if (solutions == NULL)
    {
        return;
    }

    for (int index = 0; index < solutionCount; index++)
    {
        free(solutions[index]);
    }

    free(solutions);
}

int **findSolution(
    struct CustomFunction *customFunction,
    int z,
    int *returnSize,
    int **returnColumnSizes)
{
    const int coordinateLimit = 1000;
    int **solutions = malloc(
        (size_t)coordinateLimit * sizeof(*solutions));
    int *columnSizes = malloc(
        (size_t)coordinateLimit * sizeof(*columnSizes));

    *returnSize = 0;
    *returnColumnSizes = NULL;

    if ((solutions == NULL) || (columnSizes == NULL))
    {
        free(solutions);
        free(columnSizes);
        return NULL;
    }

    int x = 1;
    int y = coordinateLimit;

    while ((x <= coordinateLimit) && (y >= 1))
    {
        int value = customFunction->f(x, y);

        if (value < z)
        {
            x++;
        }
        else if (value > z)
        {
            y--;
        }
        else
        {
            int *pair = malloc(2U * sizeof(*pair));

            if (pair == NULL)
            {
                releaseSolutions(solutions, *returnSize);
                free(columnSizes);
                *returnSize = 0;
                return NULL;
            }

            pair[0] = x;
            pair[1] = y;
            solutions[*returnSize] = pair;
            columnSizes[*returnSize] = 2;
            (*returnSize)++;
            x++;
            y--;
        }
    }

    *returnColumnSizes = columnSizes;
    return solutions;
}
