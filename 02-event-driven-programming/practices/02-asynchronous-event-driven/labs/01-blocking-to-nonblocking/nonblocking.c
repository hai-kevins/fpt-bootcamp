#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

int main(void)
{
    const uint32_t button_arrival_ms = 300UL;
    const uint32_t led_deadline_ms = 1000UL;
    uint32_t button_handled_ms = 0UL;
    bool button_handled = false;

    printf("LED ON at 0 ms\n");
    printf("LED timeout armed for 1000 ms\n");

    for (uint32_t now_ms = 0UL; now_ms <= led_deadline_ms; now_ms += 100UL)
    {
        if (!button_handled && (now_ms >= button_arrival_ms))
        {
            button_handled = true;
            button_handled_ms = now_ms;
            printf("BUTTON handled at %lu ms\n", (unsigned long) now_ms);
        }

        if (now_ms == led_deadline_ms)
        {
            printf("LED timeout handled at %lu ms\n", (unsigned long) now_ms);
        }
    }

    printf("Response latency = %lu ms\n", (unsigned long)(button_handled_ms - button_arrival_ms));

    return (button_handled_ms == button_arrival_ms) ? 0 : 1;
}
