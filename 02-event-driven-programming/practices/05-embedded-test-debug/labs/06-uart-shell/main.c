#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

static bool known_command(const char *command)
{
    const char *known[] =
    {
        "help",
        "state",
        "pool",
        "mailbox",
        "event",
        "test"
    };

    for (size_t i = 0U; i < sizeof(known) / sizeof(known[0]); i++)
    {
        if (strcmp(command, known[i]) == 0)
        {
            return true;
        }
    }

    return false;
}

int main(void)
{
    const char *commands[] =
    {
        "help",
        "state",
        "pool",
        "mailbox",
        "event",
        "test"
    };

    size_t known = 0U;

    for (size_t i = 0U; i < sizeof(commands) / sizeof(commands[0]); i++)
    {
        known += known_command(commands[i]) ? 1U : 0U;
    }

    const bool unknown = !known_command("invalid");
    const bool pass = (known == 6U) && unknown;

    (void) printf("commands=6 known=%zu unknown=%u %s\n", known, unknown ? 1U : 0U, pass ? "PASS" : "FAIL");

    return pass ? 0 : 1;
}
