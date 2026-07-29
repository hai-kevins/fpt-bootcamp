#include <stddef.h>

int lengthOfLongestSubstring(char *s)
{
    int lastPosition[256];
    int left = 0;
    int maximumLength = 0;

    for (int index = 0; index < 256; index++)
    {
        lastPosition[index] = -1;
    }

    for (int right = 0; s[right] != '\0'; right++)
    {
        unsigned char current = (unsigned char)s[right];

        if (lastPosition[current] >= left)
        {
            left = lastPosition[current] + 1;
        }

        lastPosition[current] = right;

        if ((right - left + 1) > maximumLength)
        {
            maximumLength = right - left + 1;
        }
    }

    return maximumLength;
}
