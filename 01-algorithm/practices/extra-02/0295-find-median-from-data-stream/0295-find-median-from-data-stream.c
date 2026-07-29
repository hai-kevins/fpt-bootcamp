#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct
{
    int *lower;
    int lowerSize;
    int lowerCapacity;
    int *upper;
    int upperSize;
    int upperCapacity;
} MedianFinder;

static void swapValues(int *left, int *right)
{
    int temporary = *left;
    *left = *right;
    *right = temporary;
}

static int ensureCapacity(int **data, int *capacity, int required)
{
    int newCapacity = 0;
    int *resized = NULL;

    if (*capacity >= required)
    {
        return 1;
    }

    newCapacity = (*capacity == 0) ? 16 : (*capacity * 2);

    while (newCapacity < required)
    {
        newCapacity *= 2;
    }

    resized = (int *)realloc(*data, (size_t)newCapacity * sizeof(int));

    if (resized == NULL)
    {
        return 0;
    }

    *data = resized;
    *capacity = newCapacity;
    return 1;
}

static void pushMaxHeap(int *heap, int *heapSize, int value)
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

static void pushMinHeap(int *heap, int *heapSize, int value)
{
    int index = *heapSize;
    heap[index] = value;
    (*heapSize)++;

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

static int popMaxHeap(int *heap, int *heapSize)
{
    int result = heap[0];
    int index = 0;

    (*heapSize)--;
    heap[0] = heap[*heapSize];

    while (true)
    {
        int largest = index;
        int leftChild = (2 * index) + 1;
        int rightChild = leftChild + 1;

        if ((leftChild < *heapSize) && (heap[leftChild] > heap[largest]))
        {
            largest = leftChild;
        }

        if ((rightChild < *heapSize) && (heap[rightChild] > heap[largest]))
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

    return result;
}

static int popMinHeap(int *heap, int *heapSize)
{
    int result = heap[0];
    int index = 0;

    (*heapSize)--;
    heap[0] = heap[*heapSize];

    while (true)
    {
        int smallest = index;
        int leftChild = (2 * index) + 1;
        int rightChild = leftChild + 1;

        if ((leftChild < *heapSize) && (heap[leftChild] < heap[smallest]))
        {
            smallest = leftChild;
        }

        if ((rightChild < *heapSize) && (heap[rightChild] < heap[smallest]))
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

    return result;
}

MedianFinder *medianFinderCreate(void)
{
    MedianFinder *object = (MedianFinder *)calloc(1U, sizeof(MedianFinder));
    return object;
}

void medianFinderAddNum(MedianFinder *obj, int num)
{
    if (obj == NULL)
    {
        return;
    }

    if ((obj->lowerSize == 0) || (num <= obj->lower[0]))
    {
        if (!ensureCapacity(&obj->lower,
                            &obj->lowerCapacity,
                            obj->lowerSize + 1))
        {
            return;
        }

        pushMaxHeap(obj->lower, &obj->lowerSize, num);
    }
    else
    {
        if (!ensureCapacity(&obj->upper,
                            &obj->upperCapacity,
                            obj->upperSize + 1))
        {
            return;
        }

        pushMinHeap(obj->upper, &obj->upperSize, num);
    }

    if (obj->lowerSize > (obj->upperSize + 1))
    {
        int moved = popMaxHeap(obj->lower, &obj->lowerSize);

        if (ensureCapacity(&obj->upper,
                           &obj->upperCapacity,
                           obj->upperSize + 1))
        {
            pushMinHeap(obj->upper, &obj->upperSize, moved);
        }
    }
    else if (obj->upperSize > obj->lowerSize)
    {
        int moved = popMinHeap(obj->upper, &obj->upperSize);

        if (ensureCapacity(&obj->lower,
                           &obj->lowerCapacity,
                           obj->lowerSize + 1))
        {
            pushMaxHeap(obj->lower, &obj->lowerSize, moved);
        }
    }
}

double medianFinderFindMedian(MedianFinder *obj)
{
    if ((obj == NULL) || ((obj->lowerSize + obj->upperSize) == 0))
    {
        return 0.0;
    }

    if (obj->lowerSize == obj->upperSize)
    {
        return ((double)obj->lower[0] + (double)obj->upper[0]) / 2.0;
    }

    return (double)obj->lower[0];
}

void medianFinderFree(MedianFinder *obj)
{
    if (obj != NULL)
    {
        free(obj->lower);
        free(obj->upper);
        free(obj);
    }
}
