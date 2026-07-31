#include <stdbool.h>
#include <stdio.h>

typedef struct
{
    bool timer_active;
    bool stale_timeout;
} app_t;

static void old_transition(app_t *app)
{
    app->stale_timeout = app->timer_active;
}

static void fixed_transition(app_t *app)
{
    app->timer_active = false;
    app->stale_timeout = false;
}

int main(void)
{
    app_t old_app =
    {
        .timer_active = true
    };

    old_transition(&old_app);
    const bool reproduced = old_app.stale_timeout;

    app_t fixed_app =
    {
        .timer_active = true
    };

    fixed_transition(&fixed_app);
    const bool fixed = !fixed_app.stale_timeout;

    const bool pass = reproduced && fixed;

    (void)printf(
        "bug_reproduced=%u fixed=%u stale_timeout=%u %s\n",
        reproduced ? 1U : 0U,
        fixed ? 1U : 0U,
        fixed_app.stale_timeout ? 1U : 0U,
        pass ? "PASS" : "FAIL"
    );

    return pass ? 0 : 1;
}
