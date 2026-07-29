#include <stdlib.h>
#include <string.h>

char *removeDuplicates(char *s, int k)
{
    size_t length = strlen(s);
    char *result = malloc(length + 1U);
    int *runLengths = malloc(length * sizeof(*runLengths));
    size_t top = 0U;

    if ((result == NULL) || ((length > 0U) && (runLengths == NULL)))
    {
        free(result);
        free(runLengths);
        return NULL;
    }

    for (size_t index = 0U; index < length; index++)
    {
        result[top] = s[index];

        if ((top > 0U) && (result[top - 1U] == s[index]))
        {
            runLengths[top] = runLengths[top - 1U] + 1;
        }
        else
        {
            runLengths[top] = 1;
        }

        top++;

        if (runLengths[top - 1U] == k)
        {
            top -= (size_t)k;
        }
    }

    result[top] = '\0';
    free(runLengths);
    return result;
}
