#include <limits.h>

static int maximum(int left, int right)
{
    return (left > right) ? left : right;
}

static int minimum(int left, int right)
{
    return (left < right) ? left : right;
}

double findMedianSortedArrays(
    int *nums1,
    int nums1Size,
    int *nums2,
    int nums2Size)
{
    if (nums1Size > nums2Size)
    {
        return findMedianSortedArrays(nums2, nums2Size, nums1, nums1Size);
    }

    int left = 0;
    int right = nums1Size;
    int leftPartitionSize = (nums1Size + nums2Size + 1) / 2;

    while (left <= right)
    {
        int partition1 = left + ((right - left) / 2);
        int partition2 = leftPartitionSize - partition1;

        int maxLeft1 = (partition1 == 0) ? INT_MIN : nums1[partition1 - 1];
        int minRight1 = (partition1 == nums1Size) ? INT_MAX : nums1[partition1];
        int maxLeft2 = (partition2 == 0) ? INT_MIN : nums2[partition2 - 1];
        int minRight2 = (partition2 == nums2Size) ? INT_MAX : nums2[partition2];

        if ((maxLeft1 <= minRight2) && (maxLeft2 <= minRight1))
        {
            if (((nums1Size + nums2Size) % 2) != 0)
            {
                return maximum(maxLeft1, maxLeft2);
            }

            return ((double)maximum(maxLeft1, maxLeft2) +
                    (double)minimum(minRight1, minRight2)) / 2.0;
        }

        if (maxLeft1 > minRight2)
        {
            right = partition1 - 1;
        }
        else
        {
            left = partition1 + 1;
        }
    }

    return 0.0;
}
