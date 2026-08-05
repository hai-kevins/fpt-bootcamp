#ifndef CMD_LINE_H
#define CMD_LINE_H

#include <stdbool.h>
#include <stdint.h>

#define MAX_CMD_SIZE (24U)
#define MAX_ARG_SIZE (64U)
#define MAX_CMD_TABLE_SIZE (64U)

typedef enum
{
    CMD_TBL_NOT_FOUND = 0,
    CMD_SUCCESS,
    CMD_NOT_FOUND,
    CMD_TOO_LONG,
    CMD_INVALID_INPUT,
    CMD_EMPTY,
    CMD_HANDLER_NOT_FOUND,
    CMD_INVALID_ARG,
    CMD_EXECUTION_ERROR
} cmd_line_status_t;

typedef cmd_line_status_t (*pf_cmd_func)(
    const char *arguments,
    void *context
);

typedef struct
{
    const char *cmd;
    pf_cmd_func func;
    const char *info;
} cmd_line_t;

cmd_line_status_t cmd_line_parser(
    const cmd_line_t *cmd_table,
    const char *command,
    void *context
);

bool cmd_line_argument_is_empty(const char *arguments);
bool cmd_line_argument_equals(const char *arguments, const char *expected);
bool cmd_line_parse_u32(const char *arguments, uint32_t *value);

#endif
