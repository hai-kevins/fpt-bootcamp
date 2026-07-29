#include <limits.h>
#include <stddef.h>
#include <stdlib.h>

int networkDelayTime(int **times,
                     int timesSize,
                     int *timesColSize,
                     int n,
                     int k)
{
    int *distance = NULL;
    unsigned char *visited = NULL;
    int infinity = INT_MAX / 4;
    int answer = 0;

    (void)timesColSize;

    distance = (int *)malloc((size_t)n * sizeof(int));
    visited = (unsigned char *)calloc((size_t)n, sizeof(unsigned char));

    if ((distance == NULL) || (visited == NULL))
    {
        free(distance);
        free(visited);
        return -1;
    }

    for (int node = 0; node < n; node++)
    {
        distance[node] = infinity;
    }

    distance[k - 1] = 0;

    for (int iteration = 0; iteration < n; iteration++)
    {
        int current = -1;

        for (int node = 0; node < n; node++)
        {
            if ((visited[node] == 0U) &&
                ((current == -1) || (distance[node] < distance[current])))
            {
                current = node;
            }
        }

        if ((current == -1) || (distance[current] == infinity))
        {
            break;
        }

        visited[current] = 1U;

        for (int edge = 0; edge < timesSize; edge++)
        {
            int source = times[edge][0] - 1;
            int destination = times[edge][1] - 1;
            int weight = times[edge][2];

            if ((source == current) &&
                (distance[destination] > (distance[current] + weight)))
            {
                distance[destination] = distance[current] + weight;
            }
        }
    }

    for (int node = 0; node < n; node++)
    {
        if (distance[node] == infinity)
        {
            answer = -1;
            break;
        }

        if (distance[node] > answer)
        {
            answer = distance[node];
        }
    }

    free(distance);
    free(visited);
    return answer;
}
