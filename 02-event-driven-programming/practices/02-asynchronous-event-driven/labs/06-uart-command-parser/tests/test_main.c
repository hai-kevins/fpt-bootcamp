#include "command_parser.h"
#include <stdbool.h>
#include <stdio.h>

int main(void)
{
    command_t command;
    bool pass = true;
    pass = pass && command_parse("led on", &command) && (command.type == COMMAND_LED_ON);
    pass = pass && command_parse("led off", &command) && (command.type == COMMAND_LED_OFF);
    pass = pass && command_parse("blink 500", &command) && (command.type == COMMAND_BLINK) && (command.parameter == 500UL);
    pass = pass && command_parse("status", &command) && (command.type == COMMAND_STATUS);
    pass = pass && !command_parse("blink 1", &command);
    pass = pass && !command_parse("unknown", &command);
    printf("[%s] command-parser\n", pass ? "PASS" : "FAIL");
    return pass ? 0 : 1;
}
