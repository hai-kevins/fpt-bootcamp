#include "command_parser.h"

static bool is_space(char value)
{
    return (value == ' ') || (value == '\t');
}

static bool is_digit(char value)
{
    return (value >= '0') && (value <= '9');
}

bool command_parse(const char *line, command_t *command)
{
    size_t index = 0U;
    size_t name_index = 0U;
    uint32_t value = 0UL;
    if ((line == (const char *)0) || (command == (command_t *)0))
    {
        return false;
    }
    while (is_space(line[index]))
    {
        index++;
    }
    while ((line[index] != '\0') && !is_space(line[index]))
    {
        if (name_index + 1U >= COMMAND_NAME_CAPACITY)
        {
            return false;
        }
        command->name[name_index++] = line[index++];
    }
    if (name_index == 0U)
    {
        return false;
    }
    command->name[name_index] = '\0';
    while (is_space(line[index]))
    {
        index++;
    }
    command->has_argument = false;
    command->argument = 0UL;
    if (line[index] == '\0')
    {
        return true;
    }
    while (line[index] != '\0')
    {
        const uint32_t digit = (uint32_t)(line[index] - '0');
        if (!is_digit(line[index]) || (value > (UINT32_MAX - digit) / 10UL))
        {
            return false;
        }
        value = value * 10UL + digit;
        index++;
    }
    command->has_argument = true;
    command->argument = value;
    return true;
}
