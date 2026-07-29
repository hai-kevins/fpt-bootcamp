#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int compareNumberStrings(const void *left, const void *right)
{
    const char *leftString = *(const char *const *)left;
    const char *rightString = *(const char *const *)right;
    size_t leftLength = strlen(leftString);
    size_t rightLength = strlen(rightString);
    size_t combinedLength = leftLength + rightLength;

    for (size_t index = 0U; index < combinedLength; index++)
    {
        char leftCharacter = (index < leftLength)
                                 ? leftString[index]
                                 : rightString[index - leftLength];
        char rightCharacter = (index < rightLength)
                                  ? rightString[index]
                                  : leftString[index - rightLength];

        if (leftCharacter != rightCharacter)
        {
            return (rightCharacter > leftCharacter) -
                   (rightCharacter < leftCharacter);
        }
    }

    return 0;
}

static void releaseStrings(char **strings, int count)
{
    if (strings == NULL)
    {
        return;
    }

    for (int index = 0; index < count; index++)
    {
        free(strings[index]);
    }

    free(strings);
}

char *largestNumber(int *nums, int numsSize)
{
    char **strings = calloc((size_t)numsSize, sizeof(*strings));
    size_t totalLength = 0U;

    if (strings == NULL)
    {
        return NULL;
    }

    for (int index = 0; index < numsSize; index++)
    {
        int length = snprintf(NULL, 0, "%d", nums[index]);
        strings[index] = malloc((size_t)length + 1U);

        if (strings[index] == NULL)
        {
            releaseStrings(strings, numsSize);
            return NULL;
        }

        (void)snprintf(strings[index], (size_t)length + 1U, "%d", nums[index]);
        totalLength += (size_t)length;
    }

    qsort(strings, (size_t)numsSize, sizeof(*strings), compareNumberStrings);

    if (strings[0][0] == '0')
    {
        char *zero = malloc(2U);

        if (zero != NULL)
        {
            zero[0] = '0';
            zero[1] = '\0';
        }

        releaseStrings(strings, numsSize);
        return zero;
    }

    char *result = malloc(totalLength + 1U);

    if (result == NULL)
    {
        releaseStrings(strings, numsSize);
        return NULL;
    }

    size_t writeIndex = 0U;

    for (int index = 0; index < numsSize; index++)
    {
        size_t length = strlen(strings[index]);
        memcpy(result + writeIndex, strings[index], length);
        writeIndex += length;
    }

    result[writeIndex] = '\0';
    releaseStrings(strings, numsSize);
    return result;
}
