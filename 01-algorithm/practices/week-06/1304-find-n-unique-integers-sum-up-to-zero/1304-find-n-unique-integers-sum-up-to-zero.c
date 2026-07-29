#include <stddef.h>
#include <stdlib.h>

int *sumZero(int n, int *returnSize)
{
    if (returnSize != NULL)
    {
        *returnSize = (n > 0) ? n : 0;
    }

    if (n <= 0)
    {
        return NULL;
    }

    int *result = malloc((size_t)n * sizeof(*result));

    if (result == NULL)
    {
        if (returnSize != NULL)
        {
            *returnSize = 0;
        }

        return NULL;
    }

    int index = 0;

    for (int value = 1; value <= (n / 2); value++)
    {
        result[index++] = value;
        result[index++] = -value;
    }

    if ((n % 2) != 0)
    {
        result[index] = 0;
    }

    return result;
}
