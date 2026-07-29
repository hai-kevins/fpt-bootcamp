static long long calculateHours(const int *piles, int pilesSize, int speed)
{
    long long totalHours = 0;

    for (int index = 0; index < pilesSize; index++)
    {
        totalHours += ((long long)piles[index] + speed - 1) / speed;
    }

    return totalHours;
}

int minEatingSpeed(int *piles, int pilesSize, int h)
{
    int left = 1;
    int right = 0;

    for (int index = 0; index < pilesSize; index++)
    {
        if (piles[index] > right)
        {
            right = piles[index];
        }
    }

    while (left < right)
    {
        int middle = left + ((right - left) / 2);

        if (calculateHours(piles, pilesSize, middle) <= h)
        {
            right = middle;
        }
        else
        {
            left = middle + 1;
        }
    }

    return left;
}
