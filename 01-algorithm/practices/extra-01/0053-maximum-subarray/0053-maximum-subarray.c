int maxSubArray(int *nums, int numsSize)
{
    int currentSum = nums[0];
    int maximumSum = nums[0];

    for (int index = 1; index < numsSize; index++)
    {
        currentSum = (currentSum + nums[index] > nums[index])
            ? currentSum + nums[index]
            : nums[index];

        if (currentSum > maximumSum)
        {
            maximumSum = currentSum;
        }
    }

    return maximumSum;
}
