int findMin(int *nums, int numsSize)
{
    int left = 0;
    int right = numsSize - 1;

    while (left < right)
    {
        int middle = left + ((right - left) / 2);

        if (nums[middle] > nums[right])
        {
            left = middle + 1;
        }
        else
        {
            right = middle;
        }
    }

    return nums[left];
}
