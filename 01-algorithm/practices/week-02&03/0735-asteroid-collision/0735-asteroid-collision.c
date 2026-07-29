#include <stdbool.h>
#include <stdlib.h>

int *asteroidCollision(int *asteroids, int asteroidsSize, int *returnSize)
{
    *returnSize = 0;

    if (asteroidsSize == 0)
    {
        return NULL;
    }

    int *stack = malloc((size_t)asteroidsSize * sizeof(*stack));
    int stackSize = 0;

    if (stack == NULL)
    {
        return NULL;
    }

    for (int index = 0; index < asteroidsSize; index++)
    {
        int current = asteroids[index];
        bool currentSurvives = true;

        while (currentSurvives &&
               (stackSize > 0) &&
               (stack[stackSize - 1] > 0) &&
               (current < 0))
        {
            int topMagnitude = stack[stackSize - 1];
            int currentMagnitude = -current;

            if (topMagnitude < currentMagnitude)
            {
                stackSize--;
            }
            else if (topMagnitude == currentMagnitude)
            {
                stackSize--;
                currentSurvives = false;
            }
            else
            {
                currentSurvives = false;
            }
        }

        if (currentSurvives)
        {
            stack[stackSize] = current;
            stackSize++;
        }
    }

    *returnSize = stackSize;
    return stack;
}
