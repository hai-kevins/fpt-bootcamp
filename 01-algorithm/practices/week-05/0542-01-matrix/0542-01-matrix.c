#include <stddef.h>
#include <stdlib.h>

static void freeResultMatrix(int **result, int rowCount)
{
    if (result == NULL)
    {
        return;
    }

    for (int row = 0; row < rowCount; row++)
    {
        free(result[row]);
    }

    free(result);
}

int **updateMatrix(
    int **mat,
    int matSize,
    int *matColSize,
    int *returnSize,
    int **returnColumnSizes)
{
    if ((returnSize == NULL) || (returnColumnSizes == NULL))
    {
        return NULL;
    }

    *returnSize = 0;
    *returnColumnSizes = NULL;

    if ((mat == NULL) || (matColSize == NULL) || (matSize <= 0))
    {
        return NULL;
    }

    int **distance = calloc((size_t)matSize, sizeof(*distance));
    int *columnSizes = malloc((size_t)matSize * sizeof(*columnSizes));

    if ((distance == NULL) || (columnSizes == NULL))
    {
        free(distance);
        free(columnSizes);
        return NULL;
    }

    int totalCells = 0;

    for (int row = 0; row < matSize; row++)
    {
        columnSizes[row] = matColSize[row];
        totalCells += matColSize[row];

        distance[row] = malloc((size_t)matColSize[row] * sizeof(*distance[row]));

        if ((matColSize[row] > 0) && (distance[row] == NULL))
        {
            freeResultMatrix(distance, matSize);
            free(columnSizes);
            return NULL;
        }
    }

    int *rowQueue = malloc((size_t)totalCells * sizeof(*rowQueue));
    int *columnQueue = malloc((size_t)totalCells * sizeof(*columnQueue));

    if ((rowQueue == NULL) || (columnQueue == NULL))
    {
        free(rowQueue);
        free(columnQueue);
        freeResultMatrix(distance, matSize);
        free(columnSizes);
        return NULL;
    }

    int queueHead = 0;
    int queueTail = 0;

    for (int row = 0; row < matSize; row++)
    {
        for (int column = 0; column < matColSize[row]; column++)
        {
            if (mat[row][column] == 0)
            {
                distance[row][column] = 0;
                rowQueue[queueTail] = row;
                columnQueue[queueTail] = column;
                queueTail++;
            }
            else
            {
                distance[row][column] = -1;
            }
        }
    }

    static const int rowOffset[4] = {-1, 1, 0, 0};
    static const int columnOffset[4] = {0, 0, -1, 1};

    while (queueHead < queueTail)
    {
        int currentRow = rowQueue[queueHead];
        int currentColumn = columnQueue[queueHead];
        queueHead++;

        for (int direction = 0; direction < 4; direction++)
        {
            int nextRow = currentRow + rowOffset[direction];
            int nextColumn = currentColumn + columnOffset[direction];

            if ((nextRow < 0) || (nextRow >= matSize) ||
                (nextColumn < 0) ||
                (nextColumn >= matColSize[nextRow]) ||
                (distance[nextRow][nextColumn] != -1))
            {
                continue;
            }

            distance[nextRow][nextColumn] = distance[currentRow][currentColumn] + 1;
            rowQueue[queueTail] = nextRow;
            columnQueue[queueTail] = nextColumn;
            queueTail++;
        }
    }

    free(rowQueue);
    free(columnQueue);

    *returnSize = matSize;
    *returnColumnSizes = columnSizes;
    return distance;
}
