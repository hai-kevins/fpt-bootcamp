char findTheDifference(char *s, char *t)
{
    unsigned char difference = 0U;

    for (int index = 0; s[index] != '\0'; index++)
    {
        difference ^= (unsigned char)s[index];
    }

    for (int index = 0; t[index] != '\0'; index++)
    {
        difference ^= (unsigned char)t[index];
    }

    return (char)difference;
}
