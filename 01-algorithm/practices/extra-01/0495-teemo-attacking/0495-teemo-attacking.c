int findPoisonedDuration(int *timeSeries, int timeSeriesSize, int duration)
{
    if ((timeSeriesSize <= 0) || (duration <= 0))
    {
        return 0;
    }

    int totalDuration = duration;

    for (int index = 1; index < timeSeriesSize; index++)
    {
        int gap = timeSeries[index] - timeSeries[index - 1];
        totalDuration += (gap < duration) ? gap : duration;
    }

    return totalDuration;
}
