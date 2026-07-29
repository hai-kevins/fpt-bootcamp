#include <stddef.h>

static int countLessThanOrEqual(int **matrix,
                                int matrixSize,
                                int *matrixColSize,
                                int value)
{
    int row = matrixSize - 1;
    int column = 0;
    int count = 0;
    int columnCount = matrixColSize[0];

    while ((row >= 0) && (column < columnCount))
    {
        if (matrix[row][column] <= value)
        {
            count += row + 1;
            column++;
        }
        else
        {
            row--;
        }
    }

    return count;
}

int kthSmallest(int **matrix, int matrixSize, int *matrixColSize, int k)
{
    int left = matrix[0][0];
    int right = matrix[matrixSize - 1][matrixColSize[matrixSize - 1] - 1];

    while (left < right)
    {
        int middle = left + (int)(((long long)right - left) / 2LL);
        int count = countLessThanOrEqual(matrix,
                                         matrixSize,
                                         matrixColSize,
                                         middle);

        if (count < k)
        {
            left = middle + 1;
        }
        else
        {
            right = middle;
        }
    }

    return left;
}
