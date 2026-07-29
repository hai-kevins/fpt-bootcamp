#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/*
 * LeetCode provides this structure:
 *
 * struct Employee
 * {
 *     int id;
 *     int importance;
 *     int *subordinates;
 *     int subordinatesSize;
 * };
 */

static int findEmployeeIndex(
    struct Employee **employees,
    int employeesSize,
    int id)
{
    for (int index = 0; index < employeesSize; index++)
    {
        if ((employees[index] != NULL) && (employees[index]->id == id))
        {
            return index;
        }
    }

    return -1;
}

int getImportance(struct Employee **employees, int employeesSize, int id)
{
    if ((employees == NULL) || (employeesSize <= 0))
    {
        return 0;
    }

    int startIndex = findEmployeeIndex(employees, employeesSize, id);

    if (startIndex < 0)
    {
        return 0;
    }

    int *queue = malloc((size_t)employeesSize * sizeof(*queue));
    bool *visited = calloc((size_t)employeesSize, sizeof(*visited));

    if ((queue == NULL) || (visited == NULL))
    {
        free(queue);
        free(visited);
        return 0;
    }

    int queueHead = 0;
    int queueTail = 0;
    int totalImportance = 0;

    queue[queueTail++] = startIndex;
    visited[startIndex] = true;

    while (queueHead < queueTail)
    {
        int employeeIndex = queue[queueHead++];
        struct Employee *employee = employees[employeeIndex];

        totalImportance += employee->importance;

        for (int subordinateIndex = 0;
             subordinateIndex < employee->subordinatesSize;
             subordinateIndex++)
        {
            int index = findEmployeeIndex(
                employees,
                employeesSize,
                employee->subordinates[subordinateIndex]);

            if ((index >= 0) && !visited[index])
            {
                visited[index] = true;
                queue[queueTail++] = index;
            }
        }
    }

    free(queue);
    free(visited);
    return totalImportance;
}
