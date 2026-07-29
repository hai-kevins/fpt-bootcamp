int majorityElement(int *nums, int numsSize)
{
    int candidate = 0;
    int voteCount = 0;

    for (int index = 0; index < numsSize; index++)
    {
        if (voteCount == 0)
        {
            candidate = nums[index];
        }

        if (nums[index] == candidate)
        {
            voteCount++;
        }
        else
        {
            voteCount--;
        }
    }

    return candidate;
}
