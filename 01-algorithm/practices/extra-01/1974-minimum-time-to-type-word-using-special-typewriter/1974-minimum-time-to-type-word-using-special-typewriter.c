#include <stddef.h>

int minTimeToType(char *word)
{
    if (word == NULL)
    {
        return 0;
    }

    int totalTime = 0;
    char current = 'a';

    for (int index = 0; word[index] != '\0'; index++)
    {
        int distance = word[index] - current;

        if (distance < 0)
        {
            distance = -distance;
        }

        if (distance > 13)
        {
            distance = 26 - distance;
        }

        totalTime += distance + 1;
        current = word[index];
    }

    return totalTime;
}
