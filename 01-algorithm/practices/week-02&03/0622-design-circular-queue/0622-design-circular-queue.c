#include <stdbool.h>
#include <stdlib.h>

typedef struct
{
    int *data;
    int capacity;
    int front;
    int size;
} MyCircularQueue;

MyCircularQueue *myCircularQueueCreate(int k)
{
    MyCircularQueue *queue = malloc(sizeof(*queue));

    if (queue == NULL)
    {
        return NULL;
    }

    queue->data = malloc((size_t)k * sizeof(*queue->data));

    if (queue->data == NULL)
    {
        free(queue);
        return NULL;
    }

    queue->capacity = k;
    queue->front = 0;
    queue->size = 0;
    return queue;
}

bool myCircularQueueEnQueue(MyCircularQueue *obj, int value)
{
    if ((obj == NULL) || (obj->size == obj->capacity))
    {
        return false;
    }

    int writeIndex = (obj->front + obj->size) % obj->capacity;
    obj->data[writeIndex] = value;
    obj->size++;
    return true;
}

bool myCircularQueueDeQueue(MyCircularQueue *obj)
{
    if ((obj == NULL) || (obj->size == 0))
    {
        return false;
    }

    obj->front = (obj->front + 1) % obj->capacity;
    obj->size--;
    return true;
}

int myCircularQueueFront(MyCircularQueue *obj)
{
    if ((obj == NULL) || (obj->size == 0))
    {
        return -1;
    }

    return obj->data[obj->front];
}

int myCircularQueueRear(MyCircularQueue *obj)
{
    if ((obj == NULL) || (obj->size == 0))
    {
        return -1;
    }

    int rearIndex = (obj->front + obj->size - 1) % obj->capacity;
    return obj->data[rearIndex];
}

bool myCircularQueueIsEmpty(MyCircularQueue *obj)
{
    return (obj == NULL) || (obj->size == 0);
}

bool myCircularQueueIsFull(MyCircularQueue *obj)
{
    return (obj != NULL) && (obj->size == obj->capacity);
}

void myCircularQueueFree(MyCircularQueue *obj)
{
    if (obj == NULL)
    {
        return;
    }

    free(obj->data);
    free(obj);
}
