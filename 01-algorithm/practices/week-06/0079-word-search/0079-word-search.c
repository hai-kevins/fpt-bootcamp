#include <stdbool.h>
#include <stddef.h>
#include <string.h>

static bool searchWord(
    char **board,
    int boardSize,
    int *boardColSize,
    int row,
    int column,
    const char *word,
    int wordIndex)
{
    if (word[wordIndex] == '\0')
    {
        return true;
    }

    if ((row < 0) || (row >= boardSize))
    {
        return false;
    }

    if ((column < 0) || (column >= boardColSize[row]))
    {
        return false;
    }

    if (board[row][column] != word[wordIndex])
    {
        return false;
    }

    char savedCharacter = board[row][column];
    board[row][column] = '\0';

    bool found =
        searchWord(
            board,
            boardSize,
            boardColSize,
            row - 1,
            column,
            word,
            wordIndex + 1) ||
        searchWord(
            board,
            boardSize,
            boardColSize,
            row + 1,
            column,
            word,
            wordIndex + 1) ||
        searchWord(
            board,
            boardSize,
            boardColSize,
            row,
            column - 1,
            word,
            wordIndex + 1) ||
        searchWord(
            board,
            boardSize,
            boardColSize,
            row,
            column + 1,
            word,
            wordIndex + 1);

    board[row][column] = savedCharacter;
    return found;
}

bool exist(
    char **board,
    int boardSize,
    int *boardColSize,
    char *word)
{
    if ((board == NULL) || (boardSize <= 0) ||
        (boardColSize == NULL) || (word == NULL))
    {
        return false;
    }

    size_t cellCount = 0U;

    for (int row = 0; row < boardSize; row++)
    {
        if (boardColSize[row] > 0)
        {
            cellCount += (size_t)boardColSize[row];
        }
    }

    if (strlen(word) > cellCount)
    {
        return false;
    }

    for (int row = 0; row < boardSize; row++)
    {
        for (int column = 0; column < boardColSize[row]; column++)
        {
            if (searchWord(
                    board,
                    boardSize,
                    boardColSize,
                    row,
                    column,
                    word,
                    0))
            {
                return true;
            }
        }
    }

    return false;
}
