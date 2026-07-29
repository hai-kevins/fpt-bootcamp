#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

static int compareIntegers(const void *left, const void *right)
{
    int leftValue = *(const int *)left;
    int rightValue = *(const int *)right;

    return (leftValue > rightValue) - (leftValue < rightValue);
}

bool asteroidsDestroyed(int mass, int *asteroids, int asteroidsSize)
{
    qsort(asteroids, (size_t)asteroidsSize, sizeof(*asteroids), compareIntegers);

    long long currentMass = mass;

    for (int index = 0; index < asteroidsSize; index++)
    {
        if (currentMass < asteroids[index])
        {
            return false;
        }

        currentMass += asteroids[index];
    }

    return true;
}
