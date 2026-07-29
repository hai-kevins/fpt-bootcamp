#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

static void visitProvince(
    int **isConnected,
    int cityCount,
    bool *visited,
    int city)
{
    visited[city] = true;

    for (int neighbor = 0; neighbor < cityCount; neighbor++)
    {
        if ((isConnected[city][neighbor] == 1) && !visited[neighbor])
        {
            visitProvince(isConnected, cityCount, visited, neighbor);
        }
    }
}

int findCircleNum(
    int **isConnected,
    int isConnectedSize,
    int *isConnectedColSize)
{
    (void)isConnectedColSize;

    if ((isConnected == NULL) || (isConnectedSize <= 0))
    {
        return 0;
    }

    bool *visited = calloc((size_t)isConnectedSize, sizeof(*visited));

    if (visited == NULL)
    {
        return 0;
    }

    int provinceCount = 0;

    for (int city = 0; city < isConnectedSize; city++)
    {
        if (!visited[city])
        {
            visitProvince(
                isConnected,
                isConnectedSize,
                visited,
                city);
            provinceCount++;
        }
    }

    free(visited);
    return provinceCount;
}
