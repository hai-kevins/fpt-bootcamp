#include <stddef.h>

int minAddToMakeValid(char *s)
{
    int openCount = 0;
    int additions = 0;

    if (s == NULL)
    {
        return 0;
    }

    for (int index = 0; s[index] != '\0'; index++)
    {
        if (s[index] == '(')
        {
            openCount++;
        }
        else if (openCount > 0)
        {
            openCount--;
        }
        else
        {
            additions++;
        }
    }

    return additions + openCount;
}
