#include <limits.h>

int nextGreaterElement(int n)
{
    char digits[11];
    int length = 0;
    int value = n;

    do
    {
        digits[length++] = (char)('0' + (value % 10));
        value /= 10;
    }
    while (value > 0);

    for (int left = 0, right = length - 1; left < right; left++, right--)
    {
        char temporary = digits[left];
        digits[left] = digits[right];
        digits[right] = temporary;
    }

    int pivot = length - 2;

    while ((pivot >= 0) && (digits[pivot] >= digits[pivot + 1]))
    {
        pivot--;
    }

    if (pivot < 0)
    {
        return -1;
    }

    int successor = length - 1;

    while (digits[successor] <= digits[pivot])
    {
        successor--;
    }

    char temporary = digits[pivot];
    digits[pivot] = digits[successor];
    digits[successor] = temporary;

    for (int left = pivot + 1, right = length - 1; left < right; left++, right--)
    {
        temporary = digits[left];
        digits[left] = digits[right];
        digits[right] = temporary;
    }

    long long result = 0;

    for (int index = 0; index < length; index++)
    {
        result = (result * 10) + (digits[index] - '0');

        if (result > INT_MAX)
        {
            return -1;
        }
    }

    return (int)result;
}
