#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct
{
    int *values;
    int *increments;
    int size;
    int capacity;
} CustomStack;

CustomStack *customStackCreate(int maxSize)
{
    CustomStack *stack = malloc(sizeof(*stack));

    if (stack == NULL)
    {
        return NULL;
    }

    stack->values = malloc((size_t)maxSize * sizeof(*stack->values));
    stack->increments = calloc((size_t)maxSize, sizeof(*stack->increments));
    stack->size = 0;
    stack->capacity = maxSize;

    if ((stack->values == NULL) || (stack->increments == NULL))
    {
        free(stack->values);
        free(stack->increments);
        free(stack);
        return NULL;
    }

    return stack;
}

void customStackPush(CustomStack *obj, int x)
{
    if ((obj == NULL) || (obj->size >= obj->capacity))
    {
        return;
    }

    obj->values[obj->size] = x;
    obj->increments[obj->size] = 0;
    obj->size++;
}

int customStackPop(CustomStack *obj)
{
    if ((obj == NULL) || (obj->size == 0))
    {
        return -1;
    }

    int topIndex = obj->size - 1;
    int result = obj->values[topIndex] + obj->increments[topIndex];

    if (topIndex > 0)
    {
        obj->increments[topIndex - 1] += obj->increments[topIndex];
    }

    obj->increments[topIndex] = 0;
    obj->size--;
    return result;
}

void customStackIncrement(CustomStack *obj, int k, int val)
{
    if ((obj == NULL) || (obj->size == 0) || (k <= 0))
    {
        return;
    }

    int index = (k < obj->size) ? (k - 1) : (obj->size - 1);
    obj->increments[index] += val;
}

void customStackFree(CustomStack *obj)
{
    if (obj == NULL)
    {
        return;
    }

    free(obj->values);
    free(obj->increments);
    free(obj);
}
