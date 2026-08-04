#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

int main(void)
{
    const uint32_t value = UINT32_C(0x12345678);
    const uint8_t *bytes = (const uint8_t *)(const void *)&value;

    printf("value = 0x%08" PRIX32"\n", value);
    printf("address of value = %p\n", (const void *)&value);

    for (size_t i = 0U; i < sizeof(value); i++)
    {
        printf("byte[%zu] at %p = 0x%02" PRIX8"\n", i, (const void *)&bytes[i], bytes[i]);
    }

    if (bytes[0] == UINT8_C(0x78))
    {
        printf("Detected byte order: little-endian\n");
    }
    else if (bytes[0] == UINT8_C(0x12))
    {
        printf("Detected byte order: big-endian\n");
    }
    else
    {
        printf("Detected byte order: unknown\n");
    }

    return 0;
}
