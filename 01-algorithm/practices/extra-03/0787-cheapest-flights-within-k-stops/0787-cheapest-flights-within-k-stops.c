#include <limits.h>
#include <stddef.h>
#include <stdlib.h>

int findCheapestPrice(int n,
                      int **flights,
                      int flightsSize,
                      int *flightsColSize,
                      int src,
                      int dst,
                      int k)
{
    int infinity = INT_MAX / 4;
    int *distance = (int *)malloc((size_t)n * sizeof(int));
    int *nextDistance = (int *)malloc((size_t)n * sizeof(int));

    (void)flightsColSize;

    if ((distance == NULL) || (nextDistance == NULL))
    {
        free(distance);
        free(nextDistance);
        return -1;
    }

    for (int node = 0; node < n; node++)
    {
        distance[node] = infinity;
    }

    distance[src] = 0;

    for (int edgeCount = 0; edgeCount <= k; edgeCount++)
    {
        for (int node = 0; node < n; node++)
        {
            nextDistance[node] = distance[node];
        }

        for (int edge = 0; edge < flightsSize; edge++)
        {
            int from = flights[edge][0];
            int to = flights[edge][1];
            int price = flights[edge][2];

            if ((distance[from] != infinity) &&
                (nextDistance[to] > (distance[from] + price)))
            {
                nextDistance[to] = distance[from] + price;
            }
        }

        int *temporary = distance;
        distance = nextDistance;
        nextDistance = temporary;
    }

    int result = (distance[dst] == infinity) ? -1 : distance[dst];
    free(distance);
    free(nextDistance);
    return result;
}
