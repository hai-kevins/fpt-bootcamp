#include <ctype.h>
#include <stdlib.h>
#include <string.h>

static int formsBadPair(char left, char right)
{
    return (left != right) &&
           (tolower((unsigned char)left) == tolower((unsigned char)right));
}

char *makeGood(char *s)
{
    size_t length = strlen(s);
    char *result = malloc(length + 1U);
    size_t top = 0U;

    if (result == NULL)
    {
        return NULL;
    }

    for (size_t index = 0U; index < length; index++)
    {
        if ((top > 0U) && formsBadPair(result[top - 1U], s[index]))
        {
            top--;
        }
        else
        {
            result[top] = s[index];
            top++;
        }
    }

    result[top] = '\0';
    return result;
}
