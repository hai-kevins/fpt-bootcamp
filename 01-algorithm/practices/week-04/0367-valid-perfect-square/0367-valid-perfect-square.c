#include <stdbool.h>

bool isPerfectSquare(int num)
{
    if (num < 2)
    {
        return true;
    }

    int left = 1;
    int right = num / 2;

    while (left <= right)
    {
        int middle = left + ((right - left) / 2);
        long long square = (long long)middle * middle;

        if (square == num)
        {
            return true;
        }

        if (square < num)
        {
            left = middle + 1;
        }
        else
        {
            right = middle - 1;
        }
    }

    return false;
}
