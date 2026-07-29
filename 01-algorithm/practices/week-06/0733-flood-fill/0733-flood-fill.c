#include <stddef.h>
#include <stdlib.h>

static void fillConnectedPixels(
    int **image,
    int imageSize,
    int *imageColSize,
    int row,
    int column,
    int originalColor,
    int newColor)
{
    if ((row < 0) || (row >= imageSize))
    {
        return;
    }

    if ((column < 0) || (column >= imageColSize[row]))
    {
        return;
    }

    if (image[row][column] != originalColor)
    {
        return;
    }

    image[row][column] = newColor;

    fillConnectedPixels(
        image,
        imageSize,
        imageColSize,
        row - 1,
        column,
        originalColor,
        newColor);
    fillConnectedPixels(
        image,
        imageSize,
        imageColSize,
        row + 1,
        column,
        originalColor,
        newColor);
    fillConnectedPixels(
        image,
        imageSize,
        imageColSize,
        row,
        column - 1,
        originalColor,
        newColor);
    fillConnectedPixels(
        image,
        imageSize,
        imageColSize,
        row,
        column + 1,
        originalColor,
        newColor);
}

int **floodFill(
    int **image,
    int imageSize,
    int *imageColSize,
    int sr,
    int sc,
    int color,
    int *returnSize,
    int **returnColumnSizes)
{
    if (returnSize != NULL)
    {
        *returnSize = imageSize;
    }

    if (returnColumnSizes != NULL)
    {
        *returnColumnSizes = NULL;

        if ((imageSize > 0) && (imageColSize != NULL))
        {
            int *columnSizes = malloc(
                (size_t)imageSize * sizeof(*columnSizes));

            if (columnSizes != NULL)
            {
                for (int row = 0; row < imageSize; row++)
                {
                    columnSizes[row] = imageColSize[row];
                }
            }

            *returnColumnSizes = columnSizes;
        }
    }

    if ((image == NULL) || (imageSize <= 0) ||
        (imageColSize == NULL) || (sr < 0) || (sr >= imageSize) ||
        (sc < 0) || (sc >= imageColSize[sr]))
    {
        return image;
    }

    int originalColor = image[sr][sc];

    if (originalColor == color)
    {
        return image;
    }

    fillConnectedPixels(
        image,
        imageSize,
        imageColSize,
        sr,
        sc,
        originalColor,
        color);

    return image;
}
