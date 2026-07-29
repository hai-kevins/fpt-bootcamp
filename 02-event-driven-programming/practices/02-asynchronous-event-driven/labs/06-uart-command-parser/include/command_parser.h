#ifndef LAB06_COMMAND_PARSER_H
#define LAB06_COMMAND_PARSER_H
#include <stdbool.h>
#include <stdint.h>
typedef enum
{
    COMMAND_INVALID=0,
    COMMAND_LED_ON,
    COMMAND_LED_OFF,
    COMMAND_BLINK,
    COMMAND_STATUS
} command_type_t;
typedef struct
{
    command_type_t type;
    uint32_t parameter;
} command_t;
bool command_parse(const char *line, command_t *command);
#endif
