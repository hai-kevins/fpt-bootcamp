#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

bool canVisitAllRooms(
    int **rooms,
    int roomsSize,
    int *roomsColSize)
{
    if ((rooms == NULL) || (roomsSize <= 0) || (roomsColSize == NULL))
    {
        return false;
    }

    bool *visited = calloc((size_t)roomsSize, sizeof(*visited));
    int *stack = malloc((size_t)roomsSize * sizeof(*stack));

    if ((visited == NULL) || (stack == NULL))
    {
        free(visited);
        free(stack);
        return false;
    }

    int stackSize = 0;
    int visitedCount = 1;

    visited[0] = true;
    stack[stackSize++] = 0;

    while (stackSize > 0)
    {
        int room = stack[--stackSize];

        for (int keyIndex = 0;
             keyIndex < roomsColSize[room];
             keyIndex++)
        {
            int nextRoom = rooms[room][keyIndex];

            if ((nextRoom >= 0) && (nextRoom < roomsSize) &&
                !visited[nextRoom])
            {
                visited[nextRoom] = true;
                visitedCount++;
                stack[stackSize++] = nextRoom;
            }
        }
    }

    free(visited);
    free(stack);
    return visitedCount == roomsSize;
}
