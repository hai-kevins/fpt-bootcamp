#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct
{
    int *heap;
    int heapSize;
    int capacity;
    int k;
} KthLargest;

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
    while (true)
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

int kthLargestAdd(KthLargest *obj, int val)
{
    if (obj->heapSize < obj->k)
    {
        obj->heap[obj->heapSize] = val;
        siftUp(obj->heap, obj->heapSize);
        obj->heapSize++;
    }
    else if (val > obj->heap[0])
    {
        obj->heap[0] = val;
        siftDown(obj->heap, obj->heapSize, 0);
    }

    return obj->heap[0];
}

KthLargest *kthLargestCreate(int k, int *nums, int numsSize)
{
    KthLargest *object = NULL;

    if (k <= 0)
    {
        return NULL;
    }

    object = (KthLargest *)malloc(sizeof(KthLargest));

    if (object == NULL)
    {
        return NULL;
    }

    object->heap = (int *)malloc((size_t)k * sizeof(int));

    if (object->heap == NULL)
    {
        free(object);
        return NULL;
    }

    object->heapSize = 0;
    object->capacity = k;
    object->k = k;

    for (int index = 0; index < numsSize; index++)
    {
        (void)kthLargestAdd(object, nums[index]);
    }

    return object;
}

void kthLargestFree(KthLargest *obj)
{
    if (obj != NULL)
    {
        free(obj->heap);
        free(obj);
    }
}
