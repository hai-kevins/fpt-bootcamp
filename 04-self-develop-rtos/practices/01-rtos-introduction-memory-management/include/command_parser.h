#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define COMMAND_NAME_CAPACITY (24U)

typedef struct
{
    char name[COMMAND_NAME_CAPACITY];
    bool has_argument;
    uint32_t argument;
} command_t;

bool command_parse(const char *line, command_t *command);

#endif
