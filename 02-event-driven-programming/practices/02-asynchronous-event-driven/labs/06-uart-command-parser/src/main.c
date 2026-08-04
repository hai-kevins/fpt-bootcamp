#include "command_parser.h"
#include <stdio.h>

int main(void)
{
    const char *commands[] =
    {
        "led on",
        "led off",
        "blink 500",
        "status",
        "blink abc"
    };
    for (size_t i = 0U; i < sizeof(commands) / sizeof(commands[0]); i++)
    {
        command_t command;
        const bool ok = command_parse(commands[i], &command);
        printf("%s -> ok=%d type=%d parameter=%lu\n", commands[i], ok ? 1 : 0, (int) command.type, (unsigned long) command.parameter);
    }
    return 0;
}
