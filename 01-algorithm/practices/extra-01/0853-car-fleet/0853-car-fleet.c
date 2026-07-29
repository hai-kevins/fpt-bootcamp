#include <stddef.h>
#include <stdlib.h>

typedef struct
{
    int position;
    double arrivalTime;
} Car;

static int compareCars(const void *left, const void *right)
{
    const Car *leftCar = left;
    const Car *rightCar = right;

    return (rightCar->position > leftCar->position)
        - (rightCar->position < leftCar->position);
}

int carFleet(
    int target,
    int *position,
    int positionSize,
    int *speed,
    int speedSize)
{
    (void)speedSize;

    if (positionSize <= 0)
    {
        return 0;
    }

    Car *cars = malloc((size_t)positionSize * sizeof(*cars));

    if (cars == NULL)
    {
        return 0;
    }

    for (int index = 0; index < positionSize; index++)
    {
        cars[index].position = position[index];
        cars[index].arrivalTime =
            (double)(target - position[index]) / (double)speed[index];
    }

    qsort(cars, (size_t)positionSize, sizeof(*cars), compareCars);

    int fleetCount = 0;
    double slowestTime = -1.0;

    for (int index = 0; index < positionSize; index++)
    {
        if (cars[index].arrivalTime > slowestTime)
        {
            fleetCount++;
            slowestTime = cars[index].arrivalTime;
        }
    }

    free(cars);
    return fleetCount;
}
