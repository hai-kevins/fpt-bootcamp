#include <stdint.h>

const uint8_t g_lookup_table[256] =
{
    0x00U,
    0x01U,
    0x02U,
    0x03U,
    0x04U,
    0x05U,
    0x06U,
    0x07U,
    0x08U,
    0x09U,
    0x0AU,
    0x0BU,
    0x0CU,
    0x0DU,
    0x0EU,
    0x0FU
};

uint32_t g_initialized_statistics[16] =
{
    1UL,
    2UL,
    3UL,
    4UL,
    5UL,
    6UL,
    7UL,
    8UL,
    9UL,
    10UL,
    11UL,
    12UL,
    13UL,
    14UL,
    15UL,
    16UL
};

uint8_t g_rx_buffer[512];
static uint8_t g_framebuffer[1024];

static uint32_t checksum(const uint8_t *data, uint32_t length)
{
    uint32_t result = 0UL;

    for (uint32_t i = 0UL; i < length; i++)
    {
        result = (result << 5U) ^ (result >> 27U) ^ data[i];
    }

    return result;
}

static void fill_framebuffer(void)
{
    for (uint32_t i = 0UL; i < sizeof(g_framebuffer); i++)
    {
        g_framebuffer[i] = g_lookup_table[i & 0xFFUL];
    }
}

int main(void)
{
    fill_framebuffer();

    g_initialized_statistics[0] = checksum(g_framebuffer, (uint32_t) sizeof(g_framebuffer));

    g_initialized_statistics[1] = checksum(g_rx_buffer, (uint32_t) sizeof(g_rx_buffer));

    for (;;)
    {
        __asm volatile("nop");
    }
}
