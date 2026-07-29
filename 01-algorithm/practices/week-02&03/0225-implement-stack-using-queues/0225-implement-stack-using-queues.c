#include <stdbool.h>
#include <stdlib.h>

typedef struct
{
    int *data;
    int capacity;
    int front;
    int size;
} IntQueue;

typedef struct
{
    IntQueue queue;
} MyStack;

static bool queueReserve(IntQueue *queue, int requiredCapacity)
{
    if (queue->capacity >= requiredCapacity)
    {
        return true;
    }

    int newCapacity = (queue->capacity == 0) ? 4 : queue->capacity * 2;

    while (newCapacity < requiredCapacity)
    {
        newCapacity *= 2;
    }

    int *newData = malloc((size_t)newCapacity * sizeof(*newData));

    if (newData == NULL)
    {
        return false;
    }

    for (int index = 0; index < queue->size; index++)
    {
        int oldIndex = (queue->front + index) % queue->capacity;
        newData[index] = queue->data[oldIndex];
    }

    free(queue->data);
    queue->data = newData;
    queue->capacity = newCapacity;
    queue->front = 0;
    return true;
}

static void queuePushBack(IntQueue *queue, int value)
{
    int writeIndex = (queue->front + queue->size) % queue->capacity;
    queue->data[writeIndex] = value;
    queue->size++;
}

static int queuePopFront(IntQueue *queue)
{
    int value = queue->data[queue->front];

    queue->front = (queue->front + 1) % queue->capacity;
    queue->size--;
    return value;
}

MyStack *myStackCreate(void)
{
    MyStack *stack = calloc(1U, sizeof(*stack));

    if (stack == NULL)
    {
        return NULL;
    }

    if (!queueReserve(&stack->queue, 4))
    {
        free(stack);
        return NULL;
    }

    return stack;
}

void myStackPush(MyStack *obj, int x)
{
    int oldSize = obj->queue.size;

    if (!queueReserve(&obj->queue, oldSize + 1))
    {
        return;
    }

    queuePushBack(&obj->queue, x);

    for (int rotation = 0; rotation < oldSize; rotation++)
    {
        queuePushBack(&obj->queue, queuePopFront(&obj->queue));
    }
}

int myStackPop(MyStack *obj)
{
    if ((obj == NULL) || (obj->queue.size == 0))
    {
        return -1;
    }

    return queuePopFront(&obj->queue);
}

int myStackTop(MyStack *obj)
{
    if ((obj == NULL) || (obj->queue.size == 0))
    {
        return -1;
    }

    return obj->queue.data[obj->queue.front];
}

bool myStackEmpty(MyStack *obj)
{
    return (obj == NULL) || (obj->queue.size == 0);
}

void myStackFree(MyStack *obj)
{
    if (obj == NULL)
    {
        return;
    }

    free(obj->queue.data);
    free(obj);
}
