void sortColors(int *nums, int numsSize)
{
    int low = 0;
    int current = 0;
    int high = numsSize - 1;

    while (current <= high)
    {
        if (nums[current] == 0)
        {
            int temporary = nums[low];
            nums[low] = nums[current];
            nums[current] = temporary;
            low++;
            current++;
        }
        else if (nums[current] == 1)
        {
            current++;
        }
        else
        {
            int temporary = nums[current];
            nums[current] = nums[high];
            nums[high] = temporary;
            high--;
        }
    }
}
