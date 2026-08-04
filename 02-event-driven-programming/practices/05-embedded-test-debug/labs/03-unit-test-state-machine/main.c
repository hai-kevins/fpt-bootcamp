#include <stdbool.h>
#include <stdio.h>

typedef enum
{
    IDLE = 0,
    RUNNING,
    PAUSED,
    ERROR
} state_t;

typedef enum
{
    START = 1,
    PAUSE,
    RESUME,
    STOP,
    FAIL,
    RESET,
    UNKNOWN
} signal_t;

typedef struct
{
    state_t state;
    unsigned transitions;
    unsigned unhandled;
} app_t;

static bool dispatch(app_t *app, signal_t signal)
{
    state_t next = app->state;
    bool handled = true;

    if ((app->state == IDLE) && (signal == START))
    {
        next = RUNNING;
    }
    else if ((app->state == RUNNING) && (signal == PAUSE))
    {
        next = PAUSED;
    }
    else if ((app->state == PAUSED) && (signal == RESUME))
    {
        next = RUNNING;
    }
    else if ((app->state == RUNNING) && (signal == FAIL))
    {
        next = ERROR;
    }
    else if ((app->state == ERROR) && (signal == RESET))
    {
        next = IDLE;
    }
    else if ((app->state == RUNNING) && (signal == STOP))
    {
        next = IDLE;
    }
    else
    {
        handled = false;
    }

    if (handled)
    {
        app->state = next;
        app->transitions++;
    }
    else
    {
        app->unhandled++;
    }

    return handled;
}

int main(void)
{
    app_t app =
    {
        0
    };

    bool pass = dispatch(&app, START);
    pass = pass && dispatch(&app, PAUSE);
    pass = pass && dispatch(&app, RESUME);
    pass = pass && dispatch(&app, FAIL);
    pass = pass && dispatch(&app, RESET);
    pass = pass && dispatch(&app, START);
    pass = pass && !dispatch(&app, UNKNOWN);
    pass = pass && dispatch(&app, STOP);

    (void) printf("final=%s transitions=%u unhandled=%u %s\n", app.state == IDLE ? "IDLE" : "OTHER", app.transitions,
        app.unhandled, pass ? "PASS" : "FAIL");

    return (pass && (app.state == IDLE) && (app.transitions == 7U) && (app.unhandled == 1U)) ? 0 : 1;
}
