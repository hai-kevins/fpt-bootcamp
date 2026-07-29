#include <limits.h>
#include <stddef.h>
#include <stdlib.h>

static int maximum(int left, int right)
{
    return (left > right) ? left : right;
}

static int solveLinearSlices(
    const int *slices,
    int start,
    int length,
    int slicesToChoose)
{
    const int negativeInfinity = INT_MIN / 4;
    size_t columnCount = (size_t)slicesToChoose + 1U;
    size_t cellCount = ((size_t)length + 1U) * columnCount;
    int *dp = malloc(cellCount * sizeof(*dp));

    if (dp == NULL)
    {
        return 0;
    }

    for (size_t index = 0U; index < cellCount; index++)
    {
        dp[index] = negativeInfinity;
    }

    for (int itemCount = 0; itemCount <= length; itemCount++)
    {
        dp[(size_t)itemCount * columnCount] = 0;
    }

    for (int itemCount = 1; itemCount <= length; itemCount++)
    {
        int compatibleItemCount = (itemCount >= 2) ? itemCount - 2 : 0;

        for (int chosen = 1; chosen <= slicesToChoose; chosen++)
        {
            int skip = dp[((size_t)itemCount - 1U) * columnCount +
                          (size_t)chosen];
            int previous = dp[(size_t)compatibleItemCount * columnCount +
                              (size_t)(chosen - 1)];
            int take = negativeInfinity;

            if (previous != negativeInfinity)
            {
                take = previous + slices[start + itemCount - 1];
            }

            dp[(size_t)itemCount * columnCount + (size_t)chosen] =
                maximum(skip, take);
        }
    }

    int result = dp[(size_t)length * columnCount +
                    (size_t)slicesToChoose];
    free(dp);
    return result;
}

int maxSizeSlices(int *slices, int slicesSize)
{
    int slicesToChoose = slicesSize / 3;
    int linearLength = slicesSize - 1;
    int excludeLast = solveLinearSlices(
        slices,
        0,
        linearLength,
        slicesToChoose);
    int excludeFirst = solveLinearSlices(
        slices,
        1,
        linearLength,
        slicesToChoose);

    return maximum(excludeLast, excludeFirst);
}
