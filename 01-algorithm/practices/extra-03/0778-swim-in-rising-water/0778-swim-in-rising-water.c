#include <stdbool.h>
#include <limits.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct
{
    int index;
    int time;
} HeapNode;

static void swapHeapNodes(HeapNode *left, HeapNode *right)
{
    HeapNode temporary = *left;
    *left = *right;
    *right = temporary;
}

static void pushHeap(HeapNode *heap, int *heapSize, HeapNode value)
{
    int index = *heapSize;
    heap[index] = value;
    (*heapSize)++;

    while (index > 0)
    {
        int parent = (index - 1) / 2;

        if (heap[parent].time <= heap[index].time)
        {
            break;
        }

        swapHeapNodes(&heap[parent], &heap[index]);
        index = parent;
    }
}

static HeapNode popHeap(HeapNode *heap, int *heapSize)
{
    HeapNode result = heap[0];
    int index = 0;

    (*heapSize)--;
    heap[0] = heap[*heapSize];

    while (true)
    {
        int smallest = index;
        int leftChild = (2 * index) + 1;
        int rightChild = leftChild + 1;

        if ((leftChild < *heapSize) &&
            (heap[leftChild].time < heap[smallest].time))
        {
            smallest = leftChild;
        }

        if ((rightChild < *heapSize) &&
            (heap[rightChild].time < heap[smallest].time))
        {
            smallest = rightChild;
        }

        if (smallest == index)
        {
            break;
        }

        swapHeapNodes(&heap[index], &heap[smallest]);
        index = smallest;
    }

    return result;
}

int swimInWater(int **grid, int gridSize, int *gridColSize)
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
    int *best = (int *)malloc((size_t)total * sizeof(int));
    HeapNode *heap = (HeapNode *)malloc((size_t)(total * 4 + 1) * sizeof(HeapNode));
    int heapSize = 0;
    int answer = -1;

    if ((best == NULL) || (heap == NULL))
    {
        free(best);
        free(heap);
        return -1;
    }

    for (int index = 0; index < total; index++)
    {
        best[index] = INT_MAX;
    }

    best[0] = grid[0][0];
    pushHeap(heap, &heapSize, (HeapNode){0, grid[0][0]});

    while (heapSize > 0)
    {
        HeapNode current = popHeap(heap, &heapSize);
        int row = current.index / columns;
        int column = current.index % columns;

        if (current.time != best[current.index])
        {
            continue;
        }

        if (current.index == (total - 1))
        {
            answer = current.time;
            break;
        }

        for (int direction = 0; direction < 4; direction++)
        {
            int nextRow = row + directions[direction][0];
            int nextColumn = column + directions[direction][1];

            if ((nextRow >= 0) && (nextRow < gridSize) &&
                (nextColumn >= 0) && (nextColumn < columns))
            {
                int nextIndex = (nextRow * columns) + nextColumn;
                int candidate = current.time;

                if (grid[nextRow][nextColumn] > candidate)
                {
                    candidate = grid[nextRow][nextColumn];
                }

                if (candidate < best[nextIndex])
                {
                    best[nextIndex] = candidate;
                    pushHeap(heap,
                             &heapSize,
                             (HeapNode){nextIndex, candidate});
                }
            }
        }
    }

    free(best);
    free(heap);
    return answer;
}
