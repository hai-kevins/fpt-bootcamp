int missingNumber(int *nums, int numsSize)
{
    int missing = numsSize;

    for (int index = 0; index < numsSize; index++)
    {
        missing ^= index;
        missing ^= nums[index];
    }

    return missing;
}
