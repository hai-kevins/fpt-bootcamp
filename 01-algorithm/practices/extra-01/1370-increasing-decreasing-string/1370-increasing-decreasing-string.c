#include <stddef.h>
#include <stdlib.h>
#include <string.h>

char *sortString(char *s)
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

    int frequency[26] = {0};

    for (size_t index = 0U; index < length; index++)
    {
        frequency[s[index] - 'a']++;
    }

    size_t outputIndex = 0U;

    while (outputIndex < length)
    {
        for (int letter = 0; letter < 26; letter++)
        {
            if (frequency[letter] > 0)
            {
                result[outputIndex++] = (char)('a' + letter);
                frequency[letter]--;
            }
        }

        for (int letter = 25; letter >= 0; letter--)
        {
            if (frequency[letter] > 0)
            {
                result[outputIndex++] = (char)('a' + letter);
                frequency[letter]--;
            }
        }
    }

    result[outputIndex] = '\0';
    return result;
}
