#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct
{
    int *data;
    int capacity;
    int front;
    int count;
} Deque;

static bool dequeGrow(Deque *deque)
{
    int newCapacity = deque->capacity * 2;
    int *resized = (int *)malloc((size_t)newCapacity * sizeof(int));

    if (resized == NULL)
    {
        return false;
    }

    for (int index = 0; index < deque->count; index++)
    {
        resized[index] = deque->data[(deque->front + index) % deque->capacity];
    }

    free(deque->data);
    deque->data = resized;
    deque->capacity = newCapacity;
    deque->front = 0;
    return true;
}

static bool dequePushFront(Deque *deque, int value)
{
    if ((deque->count == deque->capacity) && !dequeGrow(deque))
    {
        return false;
    }

    deque->front = (deque->front - 1 + deque->capacity) % deque->capacity;
    deque->data[deque->front] = value;
    deque->count++;
    return true;
}

static bool dequePushBack(Deque *deque, int value)
{
    int index = 0;

    if ((deque->count == deque->capacity) && !dequeGrow(deque))
    {
        return false;
    }

    index = (deque->front + deque->count) % deque->capacity;
    deque->data[index] = value;
    deque->count++;
    return true;
}

static int dequePopFront(Deque *deque)
{
    int value = deque->data[deque->front];
    deque->front = (deque->front + 1) % deque->capacity;
    deque->count--;
    return value;
}

int minimumObstacles(int **grid, int gridSize, int *gridColSize)
{
    static const int directions[4][2] =
    {
        {-1, 0},
        {1, 0},
        {0, -1},
        {0, 1}
    };
    int columns = gridColSize[0];
    int total = gridSize * columns;
    int *distance = (int *)malloc((size_t)total * sizeof(int));
    Deque deque = {NULL, (total > 0) ? total : 1, 0, 0};
    int result = -1;

    deque.data = (int *)malloc((size_t)deque.capacity * sizeof(int));

    if ((distance == NULL) || (deque.data == NULL))
    {
        free(distance);
        free(deque.data);
        return -1;
    }

    for (int index = 0; index < total; index++)
    {
        distance[index] = INT_MAX;
    }

    distance[0] = 0;

    if (!dequePushBack(&deque, 0))
    {
        free(distance);
        free(deque.data);
        return -1;
    }

    while (deque.count > 0)
    {
        int current = dequePopFront(&deque);
        int row = current / columns;
        int column = current % columns;

        for (int direction = 0; direction < 4; direction++)
        {
            int nextRow = row + directions[direction][0];
            int nextColumn = column + directions[direction][1];

            if ((nextRow >= 0) && (nextRow < gridSize) &&
                (nextColumn >= 0) && (nextColumn < columns))
            {
                int next = (nextRow * columns) + nextColumn;
                int weight = grid[nextRow][nextColumn];
                int candidate = distance[current] + weight;

                if (candidate < distance[next])
                {
                    bool pushed = false;
                    distance[next] = candidate;

                    if (weight == 0)
                    {
                        pushed = dequePushFront(&deque, next);
                    }
                    else
                    {
                        pushed = dequePushBack(&deque, next);
                    }

                    if (!pushed)
                    {
                        free(distance);
                        free(deque.data);
                        return -1;
                    }
                }
            }
        }
    }

    result = distance[total - 1];
    free(distance);
    free(deque.data);
    return result;
}
