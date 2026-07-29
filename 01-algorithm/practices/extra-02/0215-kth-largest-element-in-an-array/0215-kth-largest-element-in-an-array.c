#include <stddef.h>
#include <stdlib.h>

static void swapValues(int *left, int *right)
{
    int temporary = *left;
    *left = *right;
    *right = temporary;
}

static void siftUp(int *heap, int index)
{
    while (index > 0)
    {
        int parent = (index - 1) / 2;

        if (heap[parent] <= heap[index])
        {
            break;
        }

        swapValues(&heap[parent], &heap[index]);
        index = parent;
    }
}

static void siftDown(int *heap, int heapSize, int index)
{
    while (1)
    {
        int smallest = index;
        int leftChild = (2 * index) + 1;
        int rightChild = leftChild + 1;

        if ((leftChild < heapSize) && (heap[leftChild] < heap[smallest]))
        {
            smallest = leftChild;
        }

        if ((rightChild < heapSize) && (heap[rightChild] < heap[smallest]))
        {
            smallest = rightChild;
        }

        if (smallest == index)
        {
            break;
        }

        swapValues(&heap[index], &heap[smallest]);
        index = smallest;
    }
}

int findKthLargest(int *nums, int numsSize, int k)
{
    int *heap = NULL;
    int heapSize = 0;
    int result = 0;

    if ((nums == NULL) || (numsSize <= 0) || (k <= 0) || (k > numsSize))
    {
        return 0;
    }

    heap = (int *)malloc((size_t)k * sizeof(int));

    if (heap == NULL)
    {
        return 0;
    }

    for (int index = 0; index < numsSize; index++)
    {
        if (heapSize < k)
        {
            heap[heapSize] = nums[index];
            siftUp(heap, heapSize);
            heapSize++;
        }
        else if (nums[index] > heap[0])
        {
            heap[0] = nums[index];
            siftDown(heap, heapSize, 0);
        }
    }

    result = heap[0];
    free(heap);
    return result;
}
