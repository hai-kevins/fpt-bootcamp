#include <stdbool.h>
#include <stdlib.h>

typedef struct
{
    int *inputStack;
    int inputSize;
    int inputCapacity;
    int *outputStack;
    int outputSize;
    int outputCapacity;
} MyQueue;

static bool reserveStack(int **stack, int *capacity, int requiredCapacity)
{
    if (*capacity >= requiredCapacity)
    {
        return true;
    }

    int newCapacity = (*capacity == 0) ? 4 : (*capacity * 2);

    while (newCapacity < requiredCapacity)
    {
        newCapacity *= 2;
    }

    int *newStack = realloc(
        *stack,
        (size_t)newCapacity * sizeof(*newStack));

    if (newStack == NULL)
    {
        return false;
    }

    *stack = newStack;
    *capacity = newCapacity;
    return true;
}

static bool moveInputToOutput(MyQueue *obj)
{
    if (obj->outputSize != 0)
    {
        return true;
    }

    if (!reserveStack(
            &obj->outputStack,
            &obj->outputCapacity,
            obj->inputSize))
    {
        return false;
    }

    while (obj->inputSize > 0)
    {
        obj->inputSize--;
        obj->outputStack[obj->outputSize] = obj->inputStack[obj->inputSize];
        obj->outputSize++;
    }

    return true;
}

MyQueue *myQueueCreate(void)
{
    return calloc(1U, sizeof(MyQueue));
}

void myQueuePush(MyQueue *obj, int x)
{
    if ((obj == NULL) ||
        !reserveStack(
            &obj->inputStack,
            &obj->inputCapacity,
            obj->inputSize + 1))
    {
        return;
    }

    obj->inputStack[obj->inputSize] = x;
    obj->inputSize++;
}

int myQueuePop(MyQueue *obj)
{
    if ((obj == NULL) || !moveInputToOutput(obj) || (obj->outputSize == 0))
    {
        return -1;
    }

    obj->outputSize--;
    return obj->outputStack[obj->outputSize];
}

int myQueuePeek(MyQueue *obj)
{
    if ((obj == NULL) || !moveInputToOutput(obj) || (obj->outputSize == 0))
    {
        return -1;
    }

    return obj->outputStack[obj->outputSize - 1];
}

bool myQueueEmpty(MyQueue *obj)
{
    return (obj == NULL) ||
           ((obj->inputSize == 0) && (obj->outputSize == 0));
}

void myQueueFree(MyQueue *obj)
{
    if (obj == NULL)
    {
        return;
    }

    free(obj->inputStack);
    free(obj->outputStack);
    free(obj);
}
