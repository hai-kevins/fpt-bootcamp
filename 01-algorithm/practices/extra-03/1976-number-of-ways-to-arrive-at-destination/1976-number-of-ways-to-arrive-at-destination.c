#include <stdbool.h>
#include <limits.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct
{
    int to;
    int weight;
    int next;
} Edge;

typedef struct
{
    int node;
    long long distance;
} HeapNode;

static void swapHeapNodes(HeapNode *left, HeapNode *right)
{
    HeapNode temporary = *left;
    *left = *right;
    *right = temporary;
}

static void pushHeap(HeapNode *heap, int *heapSize, HeapNode value)
{
    int index = *heapSize;
    heap[index] = value;
    (*heapSize)++;

    while (index > 0)
    {
        int parent = (index - 1) / 2;

        if (heap[parent].distance <= heap[index].distance)
        {
            break;
        }

        swapHeapNodes(&heap[parent], &heap[index]);
        index = parent;
    }
}

static HeapNode popHeap(HeapNode *heap, int *heapSize)
{
    HeapNode result = heap[0];
    int index = 0;

    (*heapSize)--;
    heap[0] = heap[*heapSize];

    while (true)
    {
        int smallest = index;
        int leftChild = (2 * index) + 1;
        int rightChild = leftChild + 1;

        if ((leftChild < *heapSize) &&
            (heap[leftChild].distance < heap[smallest].distance))
        {
            smallest = leftChild;
        }

        if ((rightChild < *heapSize) &&
            (heap[rightChild].distance < heap[smallest].distance))
        {
            smallest = rightChild;
        }

        if (smallest == index)
        {
            break;
        }

        swapHeapNodes(&heap[index], &heap[smallest]);
        index = smallest;
    }

    return result;
}

int countPaths(int n, int **roads, int roadsSize, int *roadsColSize)
{
    const int modulo = 1000000007;
    int *head = NULL;
    Edge *adjacency = NULL;
    long long *distance = NULL;
    int *ways = NULL;
    HeapNode *heap = NULL;
    int heapSize = 0;
    int edgeCount = 0;
    int heapCapacity = (2 * roadsSize) + n + 1;
    int adjacencyCapacity = (roadsSize > 0) ? (2 * roadsSize) : 1;

    (void)roadsColSize;

    head = (int *)malloc((size_t)n * sizeof(int));
    adjacency = (Edge *)malloc((size_t)adjacencyCapacity * sizeof(Edge));
    distance = (long long *)malloc((size_t)n * sizeof(long long));
    ways = (int *)calloc((size_t)n, sizeof(int));
    heap = (HeapNode *)malloc((size_t)heapCapacity * sizeof(HeapNode));

    if ((head == NULL) || (adjacency == NULL) || (distance == NULL) ||
        (ways == NULL) || (heap == NULL))
    {
        free(head);
        free(adjacency);
        free(distance);
        free(ways);
        free(heap);
        return 0;
    }

    for (int node = 0; node < n; node++)
    {
        head[node] = -1;
        distance[node] = LLONG_MAX;
    }

    for (int index = 0; index < roadsSize; index++)
    {
        int from = roads[index][0];
        int to = roads[index][1];
        int weight = roads[index][2];

        adjacency[edgeCount] = (Edge){to, weight, head[from]};
        head[from] = edgeCount++;
        adjacency[edgeCount] = (Edge){from, weight, head[to]};
        head[to] = edgeCount++;
    }

    distance[0] = 0;
    ways[0] = 1;
    pushHeap(heap, &heapSize, (HeapNode){0, 0});

    while (heapSize > 0)
    {
        HeapNode current = popHeap(heap, &heapSize);

        if (current.distance != distance[current.node])
        {
            continue;
        }

        for (int edgeIndex = head[current.node];
             edgeIndex != -1;
             edgeIndex = adjacency[edgeIndex].next)
        {
            int nextNode = adjacency[edgeIndex].to;
            long long candidate = current.distance + adjacency[edgeIndex].weight;

            if (candidate < distance[nextNode])
            {
                distance[nextNode] = candidate;
                ways[nextNode] = ways[current.node];
                pushHeap(heap,
                         &heapSize,
                         (HeapNode){nextNode, candidate});
            }
            else if (candidate == distance[nextNode])
            {
                ways[nextNode] = (ways[nextNode] + ways[current.node]) % modulo;
            }
        }
    }

    int result = ways[n - 1];
    free(head);
    free(adjacency);
    free(distance);
    free(ways);
    free(heap);
    return result;
}
