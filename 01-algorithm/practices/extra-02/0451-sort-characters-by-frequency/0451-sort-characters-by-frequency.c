#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    unsigned char character;
    int frequency;
} CharacterFrequency;

static int compareFrequencyDescending(const void *left, const void *right)
{
    const CharacterFrequency *leftEntry = (const CharacterFrequency *)left;
    const CharacterFrequency *rightEntry = (const CharacterFrequency *)right;

    if (leftEntry->frequency != rightEntry->frequency)
    {
        return (rightEntry->frequency > leftEntry->frequency) -
               (rightEntry->frequency < leftEntry->frequency);
    }

    return (leftEntry->character > rightEntry->character) -
           (leftEntry->character < rightEntry->character);
}

char *frequencySort(char *s)
{
    int counts[256] = {0};
    CharacterFrequency entries[256];
    int entryCount = 0;
    size_t length = 0U;
    char *result = NULL;
    size_t writeIndex = 0U;

    if (s == NULL)
    {
        return NULL;
    }

    length = strlen(s);

    for (size_t index = 0U; index < length; index++)
    {
        counts[(unsigned char)s[index]]++;
    }

    for (int character = 0; character < 256; character++)
    {
        if (counts[character] > 0)
        {
            entries[entryCount].character = (unsigned char)character;
            entries[entryCount].frequency = counts[character];
            entryCount++;
        }
    }

    qsort(entries,
          (size_t)entryCount,
          sizeof(CharacterFrequency),
          compareFrequencyDescending);

    result = (char *)malloc(length + 1U);

    if (result == NULL)
    {
        return NULL;
    }

    for (int entryIndex = 0; entryIndex < entryCount; entryIndex++)
    {
        for (int count = 0; count < entries[entryIndex].frequency; count++)
        {
            result[writeIndex++] = (char)entries[entryIndex].character;
        }
    }

    result[writeIndex] = '\0';
    return result;
}
