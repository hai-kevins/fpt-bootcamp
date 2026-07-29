#include <limits.h>

int thirdMax(int *nums, int numsSize)
{
    long long first = LLONG_MIN;
    long long second = LLONG_MIN;
    long long third = LLONG_MIN;

    for (int index = 0; index < numsSize; index++)
    {
        long long current = nums[index];

        if ((current == first) || (current == second) || (current == third))
        {
            continue;
        }

        if (current > first)
        {
            third = second;
            second = first;
            first = current;
        }
        else if (current > second)
        {
            third = second;
            second = current;
        }
        else if (current > third)
        {
            third = current;
        }
    }

    return (third == LLONG_MIN) ? (int)first : (int)third;
}
