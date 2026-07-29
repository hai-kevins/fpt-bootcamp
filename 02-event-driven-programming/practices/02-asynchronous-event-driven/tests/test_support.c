#include "test.h"

#include "app.h"
#include "button.h"
#include "dispatcher.h"
#include "event_trace.h"
#include "led.h"
#include "software_timer.h"
#include "uart_service.h"

#include <stdio.h>

uint32_t g_test_passed;
uint32_t g_test_failed;

void fake_platform_reset(void);

void test_run(const char *name, test_function_t function)
{
    const bool passed = function();

    if (passed)
    {
        printf("[PASS] %s\n", name);
        g_test_passed++;
    }
}

void test_reset_framework(void)
{
    fake_platform_reset();
    event_trace_init();
    dispatcher_init();
    software_timer_init();
    app_init();
    button_init();
    led_init();
    uart_service_init();

    (void)dispatcher_register(
        EVENT_DESTINATION_APP,
        app_event_handler
    );
    (void)dispatcher_register(
        EVENT_DESTINATION_BUTTON,
        button_event_handler
    );
    (void)dispatcher_register(
        EVENT_DESTINATION_LED,
        led_event_handler
    );
    (void)dispatcher_register(
        EVENT_DESTINATION_UART,
        uart_service_event_handler
    );
}

void test_dispatch_all(void)
{
    dispatcher_run_until_empty();
}
