#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

typedef enum
{
    OFF = 0,
    BOOTING,
    IDLE,
    ACTIVE,
    ERROR
} state_t;

typedef enum
{
    BOOT = 1,
    BUTTON_PRESS,
    MODE_CHANGE,
    TIMEOUT,
    ERROR_EVENT,
    RECOVERY
} event_t;

static bool dispatch(state_t *state, event_t event)
{
    if ((*state == OFF) && (event == BOOT))
    {
        *state = BOOTING;
    }
    else if ((*state == BOOTING) && (event == BUTTON_PRESS))
    {
        *state = IDLE;
    }
    else if ((*state == IDLE) && (event == MODE_CHANGE))
    {
        *state = ACTIVE;
    }
    else if ((*state == ACTIVE) && (event == TIMEOUT))
    {
        *state = IDLE;
    }
    else if ((*state == IDLE) && (event == ERROR_EVENT))
    {
        *state = ERROR;
    }
    else if ((*state == ERROR) && (event == RECOVERY))
    {
        *state = IDLE;
    }
    else
    {
        return false;
    }

    return true;
}

int main(void)
{
    const event_t sequence[] =
    {
        BOOT,
        BUTTON_PRESS,
        MODE_CHANGE,
        TIMEOUT,
        ERROR_EVENT,
        RECOVERY
    };

    state_t state = OFF;
    bool pass = true;

    for (size_t i = 0U;
         i < sizeof(sequence) / sizeof(sequence[0]);
         i++)
    {
        pass = pass && dispatch(&state, sequence[i]);
    }

    pass = pass && (state == IDLE);

    (void)printf(
        "steps=%zu final=%s %s\n",
        sizeof(sequence) / sizeof(sequence[0]),
        state == IDLE ? "IDLE" : "OTHER",
        pass ? "PASS" : "FAIL"
    );

    return pass ? 0 : 1;
}
