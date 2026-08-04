#include <stdint.h>

const uint32_t g_firmware_version = 0x00010000UL;
const char g_build_name[] = "lab05-linker-script";

uint32_t g_initialized_counter = 100UL;
static uint32_t g_private_initialized = 42UL;

uint32_t g_uninitialized_counter;
static uint8_t g_work_buffer[256];

__attribute__((section(".noinit"))) volatile uint32_t g_reset_record;

static uint32_t checksum_bytes(const uint8_t *data, uint32_t length)
{
    uint32_t checksum = 0UL;

    for (uint32_t i = 0UL; i < length; i++)
    {
        checksum += data[i];
    }

    return checksum;
}

int main(void)
{
    g_uninitialized_counter = g_initialized_counter + g_private_initialized + g_firmware_version;

    for (uint32_t i = 0UL; i < sizeof(g_work_buffer); i++)
    {
        g_work_buffer[i] = (uint8_t) i;
    }

    g_reset_record = checksum_bytes(g_work_buffer, (uint32_t) sizeof(g_work_buffer)) + (uint32_t) g_build_name[0];

    for (;;)
    {
        __asm volatile("nop");
    }
}
