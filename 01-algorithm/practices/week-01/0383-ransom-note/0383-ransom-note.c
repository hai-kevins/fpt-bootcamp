#include <stdbool.h>

bool canConstruct(char *ransomNote, char *magazine)
{
    int availableCharacters[26] = {0};

    for (int index = 0; magazine[index] != '\0'; index++)
    {
        availableCharacters[magazine[index] - 'a']++;
    }

    for (int index = 0; ransomNote[index] != '\0'; index++)
    {
        int characterIndex = ransomNote[index] - 'a';

        if (availableCharacters[characterIndex] == 0)
        {
            return false;
        }

        availableCharacters[characterIndex]--;
    }

    return true;
}
