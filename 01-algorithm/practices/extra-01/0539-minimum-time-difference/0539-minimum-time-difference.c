#include <stdbool.h>
#include <stddef.h>

static int parseMinutes(const char *timePoint)
{
    int hours = ((timePoint[0] - '0') * 10) + (timePoint[1] - '0');
    int minutes = ((timePoint[3] - '0') * 10) + (timePoint[4] - '0');

    return (hours * 60) + minutes;
}

int findMinDifference(char **timePoints, int timePointsSize)
{
    bool present[1440] = {false};

    for (int index = 0; index < timePointsSize; index++)
    {
        int minute = parseMinutes(timePoints[index]);

        if (present[minute])
        {
            return 0;
        }

        present[minute] = true;
    }

    int first = -1;
    int previous = -1;
    int minimumDifference = 1440;

    for (int minute = 0; minute < 1440; minute++)
    {
        if (!present[minute])
        {
            continue;
        }

        if (first < 0)
        {
            first = minute;
        }

        if (previous >= 0)
        {
            int difference = minute - previous;

            if (difference < minimumDifference)
            {
                minimumDifference = difference;
            }
        }

        previous = minute;
    }

    int wrapDifference = first + 1440 - previous;

    if (wrapDifference < minimumDifference)
    {
        minimumDifference = wrapDifference;
    }

    return minimumDifference;
}
