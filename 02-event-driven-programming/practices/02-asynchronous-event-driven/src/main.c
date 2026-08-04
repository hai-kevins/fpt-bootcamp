#include "app.h"
#include "button.h"
#include "dispatcher.h"
#include "event_trace.h"
#include "led.h"
#include "platform.h"
#include "software_timer.h"
#include "uart_service.h"

int main(void)
{
    event_trace_init();
    dispatcher_init();
    software_timer_init();

    app_init();
    uart_service_init();

    (void) dispatcher_register(EVENT_DESTINATION_APP, app_event_handler);
    (void) dispatcher_register(EVENT_DESTINATION_BUTTON, button_event_handler);
    (void) dispatcher_register(EVENT_DESTINATION_LED, led_event_handler);
    (void) dispatcher_register(EVENT_DESTINATION_UART, uart_service_event_handler);

    platform_init();
    button_init();
    led_init();

    platform_uart_write_string("\r\n02 - Asynchronous Event-Driven\r\n""Button: PA0 active-low with EXTI0\r\n""LED: PC13 active-low\r\n""UART: USART1 PA9/PA10, 9600 8-N-1\r\n""Commands: help, led on, led off, blink <ms>, ""blink stop, status, trace, reset, error\r\n> ");

    (void) dispatcher_post(EVENT_SOURCE_SYSTEM, EVENT_DESTINATION_APP, SIGNAL_SYSTEM_START, 0UL, platform_time_now_ms());

    for (;;)
    {
        software_timer_process(platform_time_now_ms());

        if (!dispatcher_dispatch_once())
        {
            platform_idle();
        }
    }
}
