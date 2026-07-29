#include <stddef.h>
#include <stdlib.h>

static const int DIRECTIONS[4][2] =
{
    {-1, 0},
    {1, 0},
    {0, -1},
    {0, 1}
};

int shortestBridge(int **grid, int gridSize, int *gridColSize)
{
    if ((grid == NULL) || (gridSize <= 0) ||
        (gridColSize == NULL) || (gridColSize[0] <= 0))
    {
        return -1;
    }

    int columnCount = gridColSize[0];
    int cellCount = gridSize * columnCount;
    int *stack = malloc((size_t)cellCount * sizeof(*stack));
    int *queue = malloc((size_t)cellCount * sizeof(*queue));

    if ((stack == NULL) || (queue == NULL))
    {
        free(stack);
        free(queue);
        return -1;
    }

    int startPosition = -1;

    for (int row = 0; (row < gridSize) && (startPosition < 0); row++)
    {
        for (int column = 0; column < gridColSize[row]; column++)
        {
            if (grid[row][column] == 1)
            {
                startPosition = (row * columnCount) + column;
                break;
            }
        }
    }

    if (startPosition < 0)
    {
        free(stack);
        free(queue);
        return -1;
    }

    int stackSize = 0;
    int queueHead = 0;
    int queueTail = 0;

    stack[stackSize++] = startPosition;
    queue[queueTail++] = startPosition;
    grid[startPosition / columnCount][startPosition % columnCount] = 2;

    while (stackSize > 0)
    {
        int position = stack[--stackSize];
        int row = position / columnCount;
        int column = position % columnCount;

        for (int direction = 0; direction < 4; direction++)
        {
            int nextRow = row + DIRECTIONS[direction][0];
            int nextColumn = column + DIRECTIONS[direction][1];

            if ((nextRow < 0) || (nextRow >= gridSize) ||
                (nextColumn < 0) ||
                (nextColumn >= gridColSize[nextRow]) ||
                (grid[nextRow][nextColumn] != 1))
            {
                continue;
            }

            int nextPosition = (nextRow * columnCount) + nextColumn;

            grid[nextRow][nextColumn] = 2;
            stack[stackSize++] = nextPosition;
            queue[queueTail++] = nextPosition;
        }
    }

    int bridgeLength = 0;

    while (queueHead < queueTail)
    {
        int currentLevelSize = queueTail - queueHead;

        for (int index = 0; index < currentLevelSize; index++)
        {
            int position = queue[queueHead++];
            int row = position / columnCount;
            int column = position % columnCount;

            for (int direction = 0; direction < 4; direction++)
            {
                int nextRow = row + DIRECTIONS[direction][0];
                int nextColumn = column + DIRECTIONS[direction][1];

                if ((nextRow < 0) || (nextRow >= gridSize) ||
                    (nextColumn < 0) ||
                    (nextColumn >= gridColSize[nextRow]) ||
                    (grid[nextRow][nextColumn] == 2))
                {
                    continue;
                }

                if (grid[nextRow][nextColumn] == 1)
                {
                    free(stack);
                    free(queue);
                    return bridgeLength;
                }

                grid[nextRow][nextColumn] = 2;
                queue[queueTail++] =
                    (nextRow * columnCount) + nextColumn;
            }
        }

        bridgeLength++;
    }

    free(stack);
    free(queue);
    return -1;
}
