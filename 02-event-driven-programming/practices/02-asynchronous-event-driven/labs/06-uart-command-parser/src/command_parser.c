#include "command_parser.h"
#include <stddef.h>

static bool equal(const char *a, const char *b)
{
    if ((a == NULL) || (b == NULL))
    {
        return false;
    }
    while ((*a != '\0') && (*b != '\0'))
    {
        if (*a != *b)
        {
            return false;
        }
        a++;
        b++;
    }
    return (*a == '\0') && (*b == '\0');
}

static bool starts(const char *a, const char *b)
{
    if ((a == NULL) || (b == NULL))
    {
        return false;
    }
    while (*b != '\0')
    {
        if (*a != *b)
        {
            return false;
        }
        a++;
        b++;
    }
    return true;
}

static bool number(const char *text, uint32_t *value)
{
    uint32_t result = 0UL;
    bool any = false;
    if ((text == NULL) || (value == NULL))
    {
        return false;
    }
    while (*text != '\0')
    {
        if ((*text < '0') || (*text > '9'))
        {
            return false;
        }
        result = result * 10U + (uint32_t)(*text - '0');
        any = true;
        text++;
    }
    *value = result;
    return any;
}

bool command_parse(const char *line, command_t *command)
{
    uint32_t period;
    if (command == NULL)
    {
        return false;
    }
    command->type = COMMAND_INVALID;
    command->parameter = 0UL;
    if (equal(line, "led on"))
    {
        command->type = COMMAND_LED_ON;
    }
        else if (equal(line, "led off"))
        {
            command->type = COMMAND_LED_OFF;
        }
            else if (equal(line, "status"))
            {
                command->type = COMMAND_STATUS;
            }
                else if (starts(line, "blink ") && number(&line[6], &period) && (period >= 50UL) && (period <= 5000UL))
                {
                    command->type = COMMAND_BLINK;
                    command->parameter = period;
                }
    return command->type != COMMAND_INVALID;
}
