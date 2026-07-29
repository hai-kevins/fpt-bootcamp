int firstMissingPositive(int *nums, int numsSize)
{
    for (int index = 0; index < numsSize; index++)
    {
        while ((nums[index] >= 1) &&
               (nums[index] <= numsSize) &&
               (nums[nums[index] - 1] != nums[index]))
        {
            int targetIndex = nums[index] - 1;
            int temporary = nums[index];
            nums[index] = nums[targetIndex];
            nums[targetIndex] = temporary;
        }
    }

    for (int index = 0; index < numsSize; index++)
    {
        if (nums[index] != index + 1)
        {
            return index + 1;
        }
    }

    return numsSize + 1;
}
