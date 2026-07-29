#include <stddef.h>

void merge(int *nums1, int nums1Size, int m, int *nums2, int nums2Size, int n)
{
    int nums1Index = m - 1;
    int nums2Index = n - 1;
    int writeIndex = m + n - 1;

    (void)nums1Size;
    (void)nums2Size;

    while (nums2Index >= 0)
    {
        if ((nums1Index >= 0) && (nums1[nums1Index] > nums2[nums2Index]))
        {
            nums1[writeIndex] = nums1[nums1Index];
            nums1Index--;
        }
        else
        {
            nums1[writeIndex] = nums2[nums2Index];
            nums2Index--;
        }

        writeIndex--;
    }
}
