#include <stddef.h>
#include <stdlib.h>

int nearestExit(
    char **maze,
    int mazeSize,
    int *mazeColSize,
    int *entrance,
    int entranceSize)
{
    if ((maze == NULL) || (mazeColSize == NULL) || (entrance == NULL) ||
        (mazeSize <= 0) || (entranceSize < 2))
    {
        return -1;
    }

    int entranceRow = entrance[0];
    int entranceColumn = entrance[1];

    if ((entranceRow < 0) || (entranceRow >= mazeSize) ||
        (entranceColumn < 0) ||
        (entranceColumn >= mazeColSize[entranceRow]))
    {
        return -1;
    }

    int totalCells = 0;

    for (int row = 0; row < mazeSize; row++)
    {
        totalCells += mazeColSize[row];
    }

    int *rowQueue = malloc((size_t)totalCells * sizeof(*rowQueue));
    int *columnQueue = malloc((size_t)totalCells * sizeof(*columnQueue));

    if ((rowQueue == NULL) || (columnQueue == NULL))
    {
        free(rowQueue);
        free(columnQueue);
        return -1;
    }

    int queueHead = 0;
    int queueTail = 0;
    int distance = 0;

    maze[entranceRow][entranceColumn] = '+';
    rowQueue[queueTail] = entranceRow;
    columnQueue[queueTail] = entranceColumn;
    queueTail++;

    static const int rowOffset[4] = {-1, 1, 0, 0};
    static const int columnOffset[4] = {0, 0, -1, 1};

    while (queueHead < queueTail)
    {
        int levelSize = queueTail - queueHead;
        distance++;

        for (int levelIndex = 0; levelIndex < levelSize; levelIndex++)
        {
            int currentRow = rowQueue[queueHead];
            int currentColumn = columnQueue[queueHead];
            queueHead++;

            for (int direction = 0; direction < 4; direction++)
            {
                int nextRow = currentRow + rowOffset[direction];
                int nextColumn = currentColumn + columnOffset[direction];

                if ((nextRow < 0) || (nextRow >= mazeSize) ||
                    (nextColumn < 0) ||
                    (nextColumn >= mazeColSize[nextRow]) ||
                    (maze[nextRow][nextColumn] != '.'))
                {
                    continue;
                }

                if ((nextRow == 0) || (nextRow == mazeSize - 1) ||
                    (nextColumn == 0) ||
                    (nextColumn == mazeColSize[nextRow] - 1))
                {
                    free(rowQueue);
                    free(columnQueue);
                    return distance;
                }

                maze[nextRow][nextColumn] = '+';
                rowQueue[queueTail] = nextRow;
                columnQueue[queueTail] = nextColumn;
                queueTail++;
            }
        }
    }

    free(rowQueue);
    free(columnQueue);
    return -1;
}
