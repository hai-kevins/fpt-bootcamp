double findMaxAverage(int *nums, int numsSize, int k)
{
    long long windowSum = 0;
    long long maximumSum;

    for (int index = 0; index < k; index++)
    {
        windowSum += nums[index];
    }

    maximumSum = windowSum;

    for (int index = k; index < numsSize; index++)
    {
        windowSum += nums[index];
        windowSum -= nums[index - k];

        if (windowSum > maximumSum)
        {
            maximumSum = windowSum;
        }
    }

    return (double)maximumSum / (double)k;
}
