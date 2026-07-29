#include <stdlib.h>
#include <string.h>

typedef struct
{
    char *key;
    char **words;
    int size;
    int capacity;
} AnagramGroup;

static int compareCharacters(const void *left, const void *right)
{
    const unsigned char leftCharacter = *(const unsigned char *)left;
    const unsigned char rightCharacter = *(const unsigned char *)right;

    return (leftCharacter > rightCharacter) - (leftCharacter < rightCharacter);
}

static char *createSortedKey(const char *word)
{
    size_t length = strlen(word);
    char *key = malloc(length + 1U);

    if (key == NULL)
    {
        return NULL;
    }

    memcpy(key, word, length + 1U);
    qsort(key, length, sizeof(*key), compareCharacters);

    return key;
}

static int findGroup(AnagramGroup *groups, int groupCount, const char *key)
{
    for (int index = 0; index < groupCount; index++)
    {
        if (strcmp(groups[index].key, key) == 0)
        {
            return index;
        }
    }

    return -1;
}

static void releaseGroups(AnagramGroup *groups, int groupCount, int releaseWords)
{
    if (groups == NULL)
    {
        return;
    }

    for (int index = 0; index < groupCount; index++)
    {
        free(groups[index].key);

        if (releaseWords != 0)
        {
            free(groups[index].words);
        }
    }

    free(groups);
}

char ***groupAnagrams(char **strs, int strsSize, int *returnSize, int **returnColumnSizes)
{
    AnagramGroup *groups = NULL;
    int groupCount = 0;

    *returnSize = 0;
    *returnColumnSizes = NULL;

    if (strsSize == 0)
    {
        return NULL;
    }

    groups = calloc((size_t)strsSize, sizeof(*groups));

    if (groups == NULL)
    {
        return NULL;
    }

    for (int stringIndex = 0; stringIndex < strsSize; stringIndex++)
    {
        char *key = createSortedKey(strs[stringIndex]);
        int groupIndex;

        if (key == NULL)
        {
            releaseGroups(groups, groupCount, 1);
            return NULL;
        }

        groupIndex = findGroup(groups, groupCount, key);

        if (groupIndex < 0)
        {
            groupIndex = groupCount;
            groups[groupIndex].key = key;
            groups[groupIndex].capacity = 1;
            groups[groupIndex].words = malloc(sizeof(*groups[groupIndex].words));

            if (groups[groupIndex].words == NULL)
            {
                free(key);
                releaseGroups(groups, groupCount, 1);
                return NULL;
            }

            groupCount++;
        }
        else
        {
            free(key);
        }

        if (groups[groupIndex].size == groups[groupIndex].capacity)
        {
            int newCapacity = groups[groupIndex].capacity * 2;
            char **newWords = realloc(
                groups[groupIndex].words,
                (size_t)newCapacity * sizeof(*newWords));

            if (newWords == NULL)
            {
                releaseGroups(groups, groupCount, 1);
                return NULL;
            }

            groups[groupIndex].words = newWords;
            groups[groupIndex].capacity = newCapacity;
        }

        groups[groupIndex].words[groups[groupIndex].size] = strs[stringIndex];
        groups[groupIndex].size++;
    }

    char ***result = malloc((size_t)groupCount * sizeof(*result));
    int *columnSizes = malloc((size_t)groupCount * sizeof(*columnSizes));

    if ((result == NULL) || (columnSizes == NULL))
    {
        free(result);
        free(columnSizes);
        releaseGroups(groups, groupCount, 1);
        return NULL;
    }

    for (int groupIndex = 0; groupIndex < groupCount; groupIndex++)
    {
        result[groupIndex] = groups[groupIndex].words;
        columnSizes[groupIndex] = groups[groupIndex].size;
        free(groups[groupIndex].key);
    }

    free(groups);

    *returnSize = groupCount;
    *returnColumnSizes = columnSizes;
    return result;
}
