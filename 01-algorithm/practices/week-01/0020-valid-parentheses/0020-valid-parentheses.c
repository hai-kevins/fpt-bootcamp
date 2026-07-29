#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

bool isValid(char *s)
{
    size_t length = strlen(s);
    char *stack = malloc(length + 1U);
    size_t top = 0U;

    if (stack == NULL)
    {
        return false;
    }

    for (size_t index = 0U; index < length; index++)
    {
        char current = s[index];

        if ((current == '(') || (current == '[') || (current == '{'))
        {
            stack[top] = current;
            top++;
        }
        else
        {
            char expectedOpening;

            if (top == 0U)
            {
                free(stack);
                return false;
            }

            if (current == ')')
            {
                expectedOpening = '(';
            }
            else if (current == ']')
            {
                expectedOpening = '[';
            }
            else
            {
                expectedOpening = '{';
            }

            top--;

            if (stack[top] != expectedOpening)
            {
                free(stack);
                return false;
            }
        }
    }

    free(stack);
    return top == 0U;
}
