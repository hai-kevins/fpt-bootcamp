#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

bool isBipartite(int **graph, int graphSize, int *graphColSize)
{
    if ((graph == NULL) || (graphColSize == NULL) || (graphSize <= 0))
    {
        return true;
    }

    int *colors = calloc((size_t)graphSize, sizeof(*colors));
    int *queue = malloc((size_t)graphSize * sizeof(*queue));

    if ((colors == NULL) || (queue == NULL))
    {
        free(colors);
        free(queue);
        return false;
    }

    for (int startNode = 0; startNode < graphSize; startNode++)
    {
        if (colors[startNode] != 0)
        {
            continue;
        }

        int queueHead = 0;
        int queueTail = 0;

        colors[startNode] = 1;
        queue[queueTail++] = startNode;

        while (queueHead < queueTail)
        {
            int node = queue[queueHead++];

            for (int neighborIndex = 0;
                 neighborIndex < graphColSize[node];
                 neighborIndex++)
            {
                int neighbor = graph[node][neighborIndex];

                if ((neighbor < 0) || (neighbor >= graphSize))
                {
                    free(colors);
                    free(queue);
                    return false;
                }

                if (colors[neighbor] == 0)
                {
                    colors[neighbor] = -colors[node];
                    queue[queueTail++] = neighbor;
                }
                else if (colors[neighbor] == colors[node])
                {
                    free(colors);
                    free(queue);
                    return false;
                }
            }
        }
    }

    free(colors);
    free(queue);
    return true;
}
