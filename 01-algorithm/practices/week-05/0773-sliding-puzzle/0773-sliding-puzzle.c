#include <stdbool.h>
#include <stddef.h>

#define PUZZLE_CELL_COUNT 6
#define PUZZLE_STATE_COUNT 46656
#define PUZZLE_QUEUE_CAPACITY 720

static int encodeBoard(const int cells[PUZZLE_CELL_COUNT])
{
    int state = 0;

    for (int index = 0; index < PUZZLE_CELL_COUNT; index++)
    {
        state = (state * 6) + cells[index];
    }

    return state;
}

static void decodeBoard(int state, int cells[PUZZLE_CELL_COUNT])
{
    for (int index = PUZZLE_CELL_COUNT - 1; index >= 0; index--)
    {
        cells[index] = state % 6;
        state /= 6;
    }
}

int slidingPuzzle(int **board, int boardSize, int *boardColSize)
{
    static const int neighborCount[PUZZLE_CELL_COUNT] = {2, 3, 2, 2, 3, 2};
    static const int neighbors[PUZZLE_CELL_COUNT][3] =
    {
        {1, 3, -1},
        {0, 2, 4},
        {1, 5, -1},
        {0, 4, -1},
        {1, 3, 5},
        {2, 4, -1}
    };

    if ((board == NULL) || (boardColSize == NULL) || (boardSize != 2) ||
        (boardColSize[0] != 3) || (boardColSize[1] != 3))
    {
        return -1;
    }

    int initialCells[PUZZLE_CELL_COUNT];
    int cellIndex = 0;

    for (int row = 0; row < boardSize; row++)
    {
        for (int column = 0; column < boardColSize[row]; column++)
        {
            initialCells[cellIndex++] = board[row][column];
        }
    }

    const int targetCells[PUZZLE_CELL_COUNT] = {1, 2, 3, 4, 5, 0};
    const int targetState = encodeBoard(targetCells);
    const int initialState = encodeBoard(initialCells);

    if (initialState == targetState)
    {
        return 0;
    }

    bool visited[PUZZLE_STATE_COUNT] = {false};
    int queue[PUZZLE_QUEUE_CAPACITY];
    int queueHead = 0;
    int queueTail = 0;

    queue[queueTail++] = initialState;
    visited[initialState] = true;

    int moves = 0;

    while (queueHead < queueTail)
    {
        int levelSize = queueTail - queueHead;
        moves++;

        for (int levelIndex = 0; levelIndex < levelSize; levelIndex++)
        {
            int cells[PUZZLE_CELL_COUNT];
            decodeBoard(queue[queueHead++], cells);

            int zeroIndex = 0;

            while (cells[zeroIndex] != 0)
            {
                zeroIndex++;
            }

            for (int neighborIndex = 0;
                 neighborIndex < neighborCount[zeroIndex];
                 neighborIndex++)
            {
                int swapIndex = neighbors[zeroIndex][neighborIndex];
                int temporary = cells[zeroIndex];
                cells[zeroIndex] = cells[swapIndex];
                cells[swapIndex] = temporary;

                int nextState = encodeBoard(cells);

                if (nextState == targetState)
                {
                    return moves;
                }

                if (!visited[nextState])
                {
                    visited[nextState] = true;
                    queue[queueTail++] = nextState;
                }

                temporary = cells[zeroIndex];
                cells[zeroIndex] = cells[swapIndex];
                cells[swapIndex] = temporary;
            }
        }
    }

    return -1;
}
