#include <stdbool.h>

bool judgeCircle(char *moves)
{
    int horizontal = 0;
    int vertical = 0;

    for (int index = 0; moves[index] != '\0'; index++)
    {
        switch (moves[index])
        {
            case 'U':
                vertical++;
                break;

            case 'D':
                vertical--;
                break;

            case 'R':
                horizontal++;
                break;

            case 'L':
                horizontal--;
                break;

            default:
                break;
        }
    }

    return (horizontal == 0) && (vertical == 0);
}
