int minSubArrayLen(int target, int *nums, int numsSize)
{
    int left = 0;
    long long sum = 0;
    int minimumLength = numsSize + 1;

    for (int right = 0; right < numsSize; right++)
    {
        sum += nums[right];

        while (sum >= target)
        {
            int currentLength = right - left + 1;

            if (currentLength < minimumLength)
            {
                minimumLength = currentLength;
            }

            sum -= nums[left++];
        }
    }

    return (minimumLength == numsSize + 1) ? 0 : minimumLength;
}
