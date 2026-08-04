#include "app_sm.h"
#include <stdbool.h>
#include <stdio.h>

int main(void)
{
    app_sm_t sm;
    bool pass = true;
    app_sm_init(&sm);
    app_sm_dispatch(&sm, APP_SYSTEM_START);
    pass = pass && (sm.state == APP_IDLE);
    app_sm_dispatch(&sm, APP_BUTTON_PRESSED);
    pass = pass && (sm.state == APP_ACTIVE);
    app_sm_dispatch(&sm, APP_ERROR_DETECTED);
    pass = pass && (sm.state == APP_ERROR);
    app_sm_dispatch(&sm, APP_RESET_REQUEST);
    pass = pass && (sm.state == APP_IDLE);
    printf("[%s] product-state-machine\n", pass ? "PASS" : "FAIL");
    return pass ? 0 : 1;
}
