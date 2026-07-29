#include <stdbool.h>
#include <stddef.h>

static void swapValues(int *left, int *right)
{
    int temporary = *left;
    *left = *right;
    *right = temporary;
}

static void siftDown(int *heap, int heapSize, int index)
{
    while (true)
    {
        int largest = index;
        int leftChild = (2 * index) + 1;
        int rightChild = leftChild + 1;

        if ((leftChild < heapSize) && (heap[leftChild] > heap[largest]))
        {
            largest = leftChild;
        }

        if ((rightChild < heapSize) && (heap[rightChild] > heap[largest]))
        {
            largest = rightChild;
        }

        if (largest == index)
        {
            break;
        }

        swapValues(&heap[index], &heap[largest]);
        index = largest;
    }
}

static int popMaximum(int *heap, int *heapSize)
{
    int result = heap[0];
    (*heapSize)--;
    heap[0] = heap[*heapSize];
    siftDown(heap, *heapSize, 0);
    return result;
}

static void pushMaximum(int *heap, int *heapSize, int value)
{
    int index = *heapSize;
    heap[index] = value;
    (*heapSize)++;

    while (index > 0)
    {
        int parent = (index - 1) / 2;

        if (heap[parent] >= heap[index])
        {
            break;
        }

        swapValues(&heap[parent], &heap[index]);
        index = parent;
    }
}

int lastStoneWeight(int *stones, int stonesSize)
{
    int heapSize = stonesSize;

    for (int index = (heapSize / 2) - 1; index >= 0; index--)
    {
        siftDown(stones, heapSize, index);
    }

    while (heapSize > 1)
    {
        int heaviest = popMaximum(stones, &heapSize);
        int second = popMaximum(stones, &heapSize);

        if (heaviest != second)
        {
            pushMaximum(stones, &heapSize, heaviest - second);
        }
    }

    return (heapSize == 0) ? 0 : stones[0];
}
