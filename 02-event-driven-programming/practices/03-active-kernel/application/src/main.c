#include "ak.h"
#include "app.h"
#include "button.h"
#include "led.h"
#include "platform.h"
#include "uart.h"

int main(void)
{
    platform_init();
    led_init();
    button_init();
    uart_init();

    ak_init();
    app_init();
    app_post_start();

    uart_write_string(
        "\r\n03 Active Kernel\r\n"
        "STM32F103C8T6, HSI 8 MHz, USART1 9600\r\n"
        "Type help and press Enter.\r\n"
    );

    for (;;)
    {
        ak_process_time(platform_time_now_ms());
        if (!ak_run_once())
        {
            platform_idle();
        }
    }
}
