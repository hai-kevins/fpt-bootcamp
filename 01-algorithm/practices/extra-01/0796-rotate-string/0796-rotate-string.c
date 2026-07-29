#include <stdbool.h>
#include <stddef.h>
#include <string.h>

bool rotateString(char *s, char *goal)
{
    if ((s == NULL) || (goal == NULL))
    {
        return false;
    }

    size_t length = strlen(s);

    if (length != strlen(goal))
    {
        return false;
    }

    if (length == 0U)
    {
        return true;
    }

    for (size_t shift = 0U; shift < length; shift++)
    {
        size_t index = 0U;

        while ((index < length) &&
               (s[(shift + index) % length] == goal[index]))
        {
            index++;
        }

        if (index == length)
        {
            return true;
        }
    }

    return false;
}
