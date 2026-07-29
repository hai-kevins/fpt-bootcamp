#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* LeetCode provides struct ListNode. */

static void swapNodes(struct ListNode **left, struct ListNode **right)
{
    struct ListNode *temporary = *left;
    *left = *right;
    *right = temporary;
}

static void heapPush(struct ListNode **heap, int *heapSize, struct ListNode *node)
{
    int index = (*heapSize)++;
    heap[index] = node;

    while (index > 0)
    {
        int parent = (index - 1) / 2;

        if (heap[parent]->val <= heap[index]->val)
        {
            break;
        }

        swapNodes(&heap[parent], &heap[index]);
        index = parent;
    }
}

static struct ListNode *heapPop(struct ListNode **heap, int *heapSize)
{
    struct ListNode *minimumNode = heap[0];
    (*heapSize)--;

    if (*heapSize > 0)
    {
        heap[0] = heap[*heapSize];
        int index = 0;

        while (true)
        {
            int left = (index * 2) + 1;
            int right = left + 1;
            int smallest = index;

            if ((left < *heapSize) && (heap[left]->val < heap[smallest]->val))
            {
                smallest = left;
            }

            if ((right < *heapSize) && (heap[right]->val < heap[smallest]->val))
            {
                smallest = right;
            }

            if (smallest == index)
            {
                break;
            }

            swapNodes(&heap[index], &heap[smallest]);
            index = smallest;
        }
    }

    return minimumNode;
}

struct ListNode *mergeKLists(struct ListNode **lists, int listsSize)
{
    if (listsSize <= 0)
    {
        return NULL;
    }

    struct ListNode **heap = malloc((size_t)listsSize * sizeof(*heap));

    if (heap == NULL)
    {
        return NULL;
    }

    int heapSize = 0;

    for (int index = 0; index < listsSize; index++)
    {
        if (lists[index] != NULL)
        {
            heapPush(heap, &heapSize, lists[index]);
        }
    }

    struct ListNode sentinel = {0, NULL};
    struct ListNode *tail = &sentinel;

    while (heapSize > 0)
    {
        struct ListNode *node = heapPop(heap, &heapSize);
        tail->next = node;
        tail = node;

        if (node->next != NULL)
        {
            heapPush(heap, &heapSize, node->next);
        }
    }

    tail->next = NULL;
    free(heap);
    return sentinel.next;
}
