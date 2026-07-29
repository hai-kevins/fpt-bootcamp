#include <stdbool.h>
#include <stddef.h>

bool arrayStringsAreEqual(char **word1, int word1Size, char **word2, int word2Size)
{
    int word1Index = 0;
    int word2Index = 0;
    size_t char1Index = 0U;
    size_t char2Index = 0U;

    while ((word1Index < word1Size) && (word2Index < word2Size))
    {
        if (word1[word1Index][char1Index] != word2[word2Index][char2Index])
        {
            return false;
        }

        char1Index++;
        char2Index++;

        if (word1[word1Index][char1Index] == '\0')
        {
            word1Index++;
            char1Index = 0U;
        }

        if (word2[word2Index][char2Index] == '\0')
        {
            word2Index++;
            char2Index = 0U;
        }
    }

    return (word1Index == word1Size) && (word2Index == word2Size);
}
