#include <stddef.h>
#include <stdint.h>

void *memcpy(void *destination, const void *source, size_t count)
{
    uint8_t *dst = (uint8_t *)destination;
    const uint8_t *src = (const uint8_t *)source;

    for (size_t i = 0U; i < count; i++)
    {
        dst[i] = src[i];
    }

    return destination;
}

void *memset(void *destination, int value, size_t count)
{
    uint8_t *dst = (uint8_t *)destination;
    const uint8_t byte = (uint8_t)value;

    for (size_t i = 0U; i < count; i++)
    {
        dst[i] = byte;
    }

    return destination;
}

void __aeabi_memcpy(void *destination, const void *source, size_t count)
{
    (void)memcpy(destination, source, count);
}

void __aeabi_memcpy4(void *destination, const void *source, size_t count)
{
    (void)memcpy(destination, source, count);
}

void __aeabi_memset(void *destination, size_t count, int value)
{
    (void)memset(destination, value, count);
}

void __aeabi_memclr(void *destination, size_t count)
{
    (void)memset(destination, 0, count);
}
