#include <stdbool.h>
#include <stdint.h>

#define REG32(address) \
    (*(volatile uint32_t *)(uintptr_t)(address))

#define RCC_APB2ENR  REG32(0x40021018UL)
#define GPIOC_CRH    REG32(0x40011004UL)
#define GPIOC_BSRR   REG32(0x40011010UL)

#define IOPCEN       (1UL << 4U)
#define LED_PIN      (13U)
#define LED_SHIFT    ((LED_PIN - 8U) * 4U)
#define LED_MASK     (0xFUL << LED_SHIFT)
#define OUTPUT_2MHZ  (0x2UL << LED_SHIFT)

uint32_t g_initialized_probe = 0x12345678UL;
uint32_t g_zero_initialized_probe;
static uint8_t g_bss_buffer[32];

static void led_init(void)
{
    RCC_APB2ENR |= IOPCEN;
    GPIOC_CRH = (GPIOC_CRH & ~LED_MASK) | OUTPUT_2MHZ;
    GPIOC_BSRR = (1UL << LED_PIN);
}

static void led_toggle(void)
{
    static bool led_on;

    led_on = !led_on;

    if (led_on)
    {
        GPIOC_BSRR = (1UL << (LED_PIN + 16U));
    }
    else
    {
        GPIOC_BSRR = (1UL << LED_PIN);
    }
}

static void delay_cycles(volatile uint32_t cycles)
{
    while (cycles > 0UL)
    {
        __asm volatile("nop" : : : "memory");
        cycles--;
    }
}

int main(void)
{
    bool startup_ok = true;

    if (g_initialized_probe != 0x12345678UL)
    {
        startup_ok = false;
    }

    if (g_zero_initialized_probe != 0UL)
    {
        startup_ok = false;
    }

    for (uint32_t i = 0UL; i < sizeof(g_bss_buffer); i++)
    {
        if (g_bss_buffer[i] != 0U)
        {
            startup_ok = false;
        }
    }

    g_zero_initialized_probe = 0xA5A5A5A5UL;
    g_bss_buffer[0] = 0x5AU;

    led_init();

    for (;;)
    {
        led_toggle();
        delay_cycles(startup_ok ? 800000UL : 100000UL);
    }
}
