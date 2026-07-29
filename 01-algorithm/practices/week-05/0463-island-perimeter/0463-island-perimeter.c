int islandPerimeter(int **grid, int gridSize, int *gridColSize)
{
    if ((grid == 0) || (gridColSize == 0) || (gridSize <= 0))
    {
        return 0;
    }

    int perimeter = 0;

    for (int row = 0; row < gridSize; row++)
    {
        for (int column = 0; column < gridColSize[row]; column++)
        {
            if (grid[row][column] == 0)
            {
                continue;
            }

            perimeter += 4;

            if ((row > 0) && (column < gridColSize[row - 1]) &&
                (grid[row - 1][column] == 1))
            {
                perimeter -= 2;
            }

            if ((column > 0) && (grid[row][column - 1] == 1))
            {
                perimeter -= 2;
            }
        }
    }

    return perimeter;
}
