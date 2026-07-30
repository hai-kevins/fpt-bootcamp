#include "test.h"
#include "ak.h"
#include "app.h"

extern bool g_fake_led_on;

bool test_application_flow(void)
{
    ak_init();
    app_init();
    app_post_start();
    (void)ak_run_until_idle(32U);
    TEST_ASSERT_EQ(1U, app_state_get());
    app_isr_button_edge();
    /* Button debounce is covered by timer/task tests; app starts idle. */
    TEST_ASSERT_TRUE(!g_fake_led_on);
    return true;
}
