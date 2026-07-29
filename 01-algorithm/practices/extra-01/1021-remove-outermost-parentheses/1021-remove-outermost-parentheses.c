#include <stddef.h>
#include <stdlib.h>
#include <string.h>

char *removeOuterParentheses(char *s)
{
    if (s == NULL)
    {
        return NULL;
    }

    size_t length = strlen(s);
    char *result = malloc(length + 1U);

    if (result == NULL)
    {
        return NULL;
    }

    int depth = 0;
    size_t outputIndex = 0U;

    for (size_t index = 0U; index < length; index++)
    {
        if (s[index] == '(')
        {
            if (depth > 0)
            {
                result[outputIndex++] = s[index];
            }

            depth++;
        }
        else
        {
            depth--;

            if (depth > 0)
            {
                result[outputIndex++] = s[index];
            }
        }
    }

    result[outputIndex] = '\0';
    return result;
}
