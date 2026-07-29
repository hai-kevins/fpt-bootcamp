#include <stddef.h>
#include <stdlib.h>

typedef struct LinkedListNode
{
    int value;
    struct LinkedListNode *next;
} LinkedListNode;

typedef struct
{
    LinkedListNode sentinel;
    int size;
} MyLinkedList;

MyLinkedList *myLinkedListCreate(void)
{
    MyLinkedList *list = malloc(sizeof(*list));

    if (list == NULL)
    {
        return NULL;
    }

    list->sentinel.value = 0;
    list->sentinel.next = NULL;
    list->size = 0;
    return list;
}

int myLinkedListGet(MyLinkedList *obj, int index)
{
    if ((obj == NULL) || (index < 0) || (index >= obj->size))
    {
        return -1;
    }

    LinkedListNode *node = obj->sentinel.next;

    for (int current = 0; current < index; current++)
    {
        node = node->next;
    }

    return node->value;
}

void myLinkedListAddAtIndex(MyLinkedList *obj, int index, int val)
{
    if ((obj == NULL) || (index > obj->size))
    {
        return;
    }

    if (index < 0)
    {
        index = 0;
    }

    LinkedListNode *previous = &obj->sentinel;

    for (int current = 0; current < index; current++)
    {
        previous = previous->next;
    }

    LinkedListNode *node = malloc(sizeof(*node));

    if (node == NULL)
    {
        return;
    }

    node->value = val;
    node->next = previous->next;
    previous->next = node;
    obj->size++;
}

void myLinkedListAddAtHead(MyLinkedList *obj, int val)
{
    myLinkedListAddAtIndex(obj, 0, val);
}

void myLinkedListAddAtTail(MyLinkedList *obj, int val)
{
    if (obj != NULL)
    {
        myLinkedListAddAtIndex(obj, obj->size, val);
    }
}

void myLinkedListDeleteAtIndex(MyLinkedList *obj, int index)
{
    if ((obj == NULL) || (index < 0) || (index >= obj->size))
    {
        return;
    }

    LinkedListNode *previous = &obj->sentinel;

    for (int current = 0; current < index; current++)
    {
        previous = previous->next;
    }

    LinkedListNode *removedNode = previous->next;
    previous->next = removedNode->next;
    free(removedNode);
    obj->size--;
}

void myLinkedListFree(MyLinkedList *obj)
{
    if (obj == NULL)
    {
        return;
    }

    LinkedListNode *node = obj->sentinel.next;

    while (node != NULL)
    {
        LinkedListNode *nextNode = node->next;
        free(node);
        node = nextNode;
    }

    free(obj);
}
