#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct
{
    int to;
    double probability;
    int next;
} Edge;

typedef struct
{
    int node;
    double probability;
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

        if (heap[parent].probability >= heap[index].probability)
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
        int largest = index;
        int leftChild = (2 * index) + 1;
        int rightChild = leftChild + 1;

        if ((leftChild < *heapSize) &&
            (heap[leftChild].probability > heap[largest].probability))
        {
            largest = leftChild;
        }

        if ((rightChild < *heapSize) &&
            (heap[rightChild].probability > heap[largest].probability))
        {
            largest = rightChild;
        }

        if (largest == index)
        {
            break;
        }

        swapHeapNodes(&heap[index], &heap[largest]);
        index = largest;
    }

    return result;
}

double maxProbability(int n,
                      int **edges,
                      int edgesSize,
                      int *edgesColSize,
                      double *succProb,
                      int succProbSize,
                      int startNode,
                      int endNode)
{
    int *head = NULL;
    Edge *adjacency = NULL;
    double *best = NULL;
    HeapNode *heap = NULL;
    int heapSize = 0;
    int edgeCount = 0;
    int adjacencyCapacity = (edgesSize > 0) ? (2 * edgesSize) : 1;
    double answer = 0.0;

    (void)edgesColSize;
    (void)succProbSize;

    if (startNode == endNode)
    {
        return 1.0;
    }

    head = (int *)malloc((size_t)n * sizeof(int));
    adjacency = (Edge *)malloc((size_t)adjacencyCapacity * sizeof(Edge));
    best = (double *)calloc((size_t)n, sizeof(double));
    heap = (HeapNode *)malloc((size_t)((2 * edgesSize) + 1) * sizeof(HeapNode));

    if ((head == NULL) || (adjacency == NULL) || (best == NULL) || (heap == NULL))
    {
        free(head);
        free(adjacency);
        free(best);
        free(heap);
        return 0.0;
    }

    for (int node = 0; node < n; node++)
    {
        head[node] = -1;
    }

    for (int index = 0; index < edgesSize; index++)
    {
        int from = edges[index][0];
        int to = edges[index][1];

        adjacency[edgeCount].to = to;
        adjacency[edgeCount].probability = succProb[index];
        adjacency[edgeCount].next = head[from];
        head[from] = edgeCount++;

        adjacency[edgeCount].to = from;
        adjacency[edgeCount].probability = succProb[index];
        adjacency[edgeCount].next = head[to];
        head[to] = edgeCount++;
    }

    best[startNode] = 1.0;
    pushHeap(heap, &heapSize, (HeapNode){startNode, 1.0});

    while (heapSize > 0)
    {
        HeapNode current = popHeap(heap, &heapSize);

        if (current.probability < best[current.node])
        {
            continue;
        }

        if (current.node == endNode)
        {
            answer = current.probability;
            break;
        }

        for (int edgeIndex = head[current.node];
             edgeIndex != -1;
             edgeIndex = adjacency[edgeIndex].next)
        {
            int nextNode = adjacency[edgeIndex].to;
            double candidate = current.probability *
                               adjacency[edgeIndex].probability;

            if (candidate > best[nextNode])
            {
                best[nextNode] = candidate;
                pushHeap(heap,
                         &heapSize,
                         (HeapNode){nextNode, candidate});
            }
        }
    }

    free(head);
    free(adjacency);
    free(best);
    free(heap);
    return answer;
}
