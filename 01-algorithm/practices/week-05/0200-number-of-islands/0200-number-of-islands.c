#include <stddef.h>
#include <stdlib.h>

int numIslands(char **grid, int gridSize, int *gridColSize)
{
    if ((grid == NULL) || (gridColSize == NULL) || (gridSize <= 0))
    {
        return 0;
    }

    int totalCells = 0;

    for (int row = 0; row < gridSize; row++)
    {
        totalCells += gridColSize[row];
    }

    int *rowQueue = malloc((size_t)totalCells * sizeof(*rowQueue));
    int *columnQueue = malloc((size_t)totalCells * sizeof(*columnQueue));

    if ((rowQueue == NULL) || (columnQueue == NULL))
    {
        free(rowQueue);
        free(columnQueue);
        return 0;
    }

    static const int rowOffset[4] = {-1, 1, 0, 0};
    static const int columnOffset[4] = {0, 0, -1, 1};
    int islandCount = 0;

    for (int row = 0; row < gridSize; row++)
    {
        for (int column = 0; column < gridColSize[row]; column++)
        {
            if (grid[row][column] != '1')
            {
                continue;
            }

            islandCount++;
            int queueHead = 0;
            int queueTail = 0;

            grid[row][column] = '0';
            rowQueue[queueTail] = row;
            columnQueue[queueTail] = column;
            queueTail++;

            while (queueHead < queueTail)
            {
                int currentRow = rowQueue[queueHead];
                int currentColumn = columnQueue[queueHead];
                queueHead++;

                for (int direction = 0; direction < 4; direction++)
                {
                    int nextRow = currentRow + rowOffset[direction];
                    int nextColumn = currentColumn + columnOffset[direction];

                    if ((nextRow < 0) || (nextRow >= gridSize) ||
                        (nextColumn < 0) ||
                        (nextColumn >= gridColSize[nextRow]) ||
                        (grid[nextRow][nextColumn] != '1'))
                    {
                        continue;
                    }

                    grid[nextRow][nextColumn] = '0';
                    rowQueue[queueTail] = nextRow;
                    columnQueue[queueTail] = nextColumn;
                    queueTail++;
                }
            }
        }
    }

    free(rowQueue);
    free(columnQueue);
    return islandCount;
}
