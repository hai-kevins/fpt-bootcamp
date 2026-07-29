#include <stdlib.h>
#include <string.h>

static int isOperator(const char *token)
{
    return (token[1] == '\0') &&
           ((token[0] == '+') ||
            (token[0] == '-') ||
            (token[0] == '*') ||
            (token[0] == '/'));
}

int evalRPN(char **tokens, int tokensSize)
{
    int *stack = malloc((size_t)tokensSize * sizeof(*stack));
    int stackSize = 0;

    if (stack == NULL)
    {
        return 0;
    }

    for (int index = 0; index < tokensSize; index++)
    {
        if (!isOperator(tokens[index]))
        {
            stack[stackSize] = (int)strtol(tokens[index], NULL, 10);
            stackSize++;
            continue;
        }

        int right = stack[stackSize - 1];
        int left = stack[stackSize - 2];
        stackSize -= 2;

        if (tokens[index][0] == '+')
        {
            stack[stackSize] = left + right;
        }
        else if (tokens[index][0] == '-')
        {
            stack[stackSize] = left - right;
        }
        else if (tokens[index][0] == '*')
        {
            stack[stackSize] = left * right;
        }
        else
        {
            stack[stackSize] = left / right;
        }

        stackSize++;
    }

    int result = stack[0];
    free(stack);
    return result;
}
