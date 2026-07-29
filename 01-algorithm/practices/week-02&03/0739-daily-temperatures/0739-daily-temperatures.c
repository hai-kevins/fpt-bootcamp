#include <stdlib.h>

int *dailyTemperatures(
    int *temperatures,
    int temperaturesSize,
    int *returnSize)
{
    *returnSize = temperaturesSize;

    if (temperaturesSize == 0)
    {
        return NULL;
    }

    int *result = calloc((size_t)temperaturesSize, sizeof(*result));
    int *stack = malloc((size_t)temperaturesSize * sizeof(*stack));
    int stackSize = 0;

    if ((result == NULL) || (stack == NULL))
    {
        free(result);
        free(stack);
        *returnSize = 0;
        return NULL;
    }

    for (int index = 0; index < temperaturesSize; index++)
    {
        while ((stackSize > 0) &&
               (temperatures[index] > temperatures[stack[stackSize - 1]]))
        {
            int previousIndex = stack[stackSize - 1];
            stackSize--;
            result[previousIndex] = index - previousIndex;
        }

        stack[stackSize] = index;
        stackSize++;
    }

    free(stack);
    return result;
}
