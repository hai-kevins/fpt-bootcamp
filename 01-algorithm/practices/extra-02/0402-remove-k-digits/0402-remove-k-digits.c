#include <stddef.h>
#include <stdlib.h>
#include <string.h>

char *removeKdigits(char *num, int k)
{
    size_t length = 0U;
    char *stack = NULL;
    size_t stackSize = 0U;
    size_t firstNonZero = 0U;
    size_t resultLength = 0U;
    char *result = NULL;

    if (num == NULL)
    {
        return NULL;
    }

    length = strlen(num);
    stack = (char *)malloc(length + 1U);

    if (stack == NULL)
    {
        return NULL;
    }

    for (size_t index = 0U; index < length; index++)
    {
        while ((k > 0) && (stackSize > 0U) &&
               (stack[stackSize - 1U] > num[index]))
        {
            stackSize--;
            k--;
        }

        stack[stackSize++] = num[index];
    }

    while ((k > 0) && (stackSize > 0U))
    {
        stackSize--;
        k--;
    }

    while ((firstNonZero < stackSize) && (stack[firstNonZero] == '0'))
    {
        firstNonZero++;
    }

    resultLength = stackSize - firstNonZero;

    if (resultLength == 0U)
    {
        result = (char *)malloc(2U);

        if (result != NULL)
        {
            result[0] = '0';
            result[1] = '\0';
        }
    }
    else
    {
        result = (char *)malloc(resultLength + 1U);

        if (result != NULL)
        {
            memcpy(result, &stack[firstNonZero], resultLength);
            result[resultLength] = '\0';
        }
    }

    free(stack);
    return result;
}
