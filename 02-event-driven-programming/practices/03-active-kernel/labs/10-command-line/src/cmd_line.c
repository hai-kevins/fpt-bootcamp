#include "cmd_line.h"

#include <stddef.h>

static bool is_horizontal_space(char value)
{
    return (value == ' ') || (value == '\t');
}

static bool is_line_end(char value)
{
    return (value == '\0') || (value == '\r') || (value == '\n');
}

static bool string_equal(const char *left, const char *right)
{
    if ((left == NULL) || (right == NULL))
    {
        return false;
    }

    while ((*left != '\0') && (*right != '\0'))
    {
        if (*left != *right)
        {
            return false;
        }

        left++;
        right++;
    }

    return (*left == '\0') && (*right == '\0');
}

cmd_line_status_t cmd_line_parser(
    const cmd_line_t *cmd_table,
    const char *command,
    void *context
)
{
    char command_name[MAX_CMD_SIZE];
    char arguments[MAX_ARG_SIZE];
    size_t command_index = 0U;
    size_t argument_index = 0U;
    size_t table_index;

    if (cmd_table == NULL)
    {
        return CMD_TBL_NOT_FOUND;
    }

    if (command == NULL)
    {
        return CMD_INVALID_INPUT;
    }

    while (is_horizontal_space(*command))
    {
        command++;
    }

    if (is_line_end(*command))
    {
        return CMD_EMPTY;
    }

    while (!is_line_end(*command) && !is_horizontal_space(*command))
    {
        if ((command_index + 1U) >= MAX_CMD_SIZE)
        {
            return CMD_TOO_LONG;
        }

        command_name[command_index] = *command;
        command_index++;
        command++;
    }

    command_name[command_index] = '\0';

    while (is_horizontal_space(*command))
    {
        command++;
    }

    while (!is_line_end(*command))
    {
        if ((argument_index + 1U) >= MAX_ARG_SIZE)
        {
            return CMD_TOO_LONG;
        }

        arguments[argument_index] = *command;
        argument_index++;
        command++;
    }

    while ((argument_index > 0U) &&
           is_horizontal_space(arguments[argument_index - 1U]))
    {
        argument_index--;
    }

    arguments[argument_index] = '\0';

    for (table_index = 0U;
         table_index < MAX_CMD_TABLE_SIZE;
         table_index++)
    {
        if (cmd_table[table_index].cmd == NULL)
        {
            return CMD_NOT_FOUND;
        }

        if (string_equal(cmd_table[table_index].cmd, command_name))
        {
            if (cmd_table[table_index].func == NULL)
            {
                return CMD_HANDLER_NOT_FOUND;
            }

            return cmd_table[table_index].func(arguments, context);
        }
    }

    return CMD_TBL_NOT_FOUND;
}

bool cmd_line_argument_is_empty(const char *arguments)
{
    return (arguments != NULL) && (arguments[0] == '\0');
}

bool cmd_line_argument_equals(const char *arguments, const char *expected)
{
    return string_equal(arguments, expected);
}

bool cmd_line_parse_u32(const char *arguments, uint32_t *value)
{
    uint32_t result = 0U;
    bool has_digit = false;

    if ((arguments == NULL) || (value == NULL))
    {
        return false;
    }

    while (is_horizontal_space(*arguments))
    {
        arguments++;
    }

    while ((*arguments >= '0') && (*arguments <= '9'))
    {
        const uint32_t digit = (uint32_t)(*arguments - '0');

        if (result > ((UINT32_MAX - digit) / 10U))
        {
            return false;
        }

        result = (result * 10U) + digit;
        has_digit = true;
        arguments++;
    }

    while (is_horizontal_space(*arguments))
    {
        arguments++;
    }

    if (!has_digit || (*arguments != '\0'))
    {
        return false;
    }

    *value = result;

    return true;
}
