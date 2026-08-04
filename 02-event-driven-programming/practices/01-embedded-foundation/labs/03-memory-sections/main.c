#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

const uint32_t g_firmware_version = UINT32_C(0x00010000);
uint32_t g_initialized_counter = 100U;
uint32_t g_uninitialized_counter;
static uint8_t g_static_buffer[128];
static uint32_t g_private_value = 42U;

int main(void)
{
    g_uninitialized_counter++;
    g_static_buffer[0] = UINT8_C(0xA5);

    printf("firmware version      : 0x%08" PRIX32"\n", g_firmware_version);
    printf("initialized counter   : %" PRIu32"\n", g_initialized_counter);
    printf("uninitialized counter : %" PRIu32"\n", g_uninitialized_counter);
    printf("static buffer[0]       : 0x%02" PRIX8"\n", g_static_buffer[0]);
    printf("private value          : %" PRIu32"\n", g_private_value);

    return 0;
}
