#include <stddef.h>
#include <stdint.h>

void *memcpy(void *destination, const void *source, size_t count)
{
    uint8_t *dst = (uint8_t *)destination;
    const uint8_t *src = (const uint8_t *)source;

    while (count > 0U)
    {
        *dst = *src;
        ++dst;
        ++src;
        --count;
    }

    return destination;
}

void *memset(void *destination, int value, size_t count)
{
    uint8_t *dst = (uint8_t *)destination;

    while (count > 0U)
    {
        *dst = (uint8_t)value;
        ++dst;
        --count;
    }

    return destination;
}

void *memmove(void *destination, const void *source, size_t count)
{
    uint8_t *dst = (uint8_t *)destination;
    const uint8_t *src = (const uint8_t *)source;

    if (dst < src)
    {
        while (count > 0U)
        {
            *dst = *src;
            ++dst;
            ++src;
            --count;
        }
    }
    else if (dst > src)
    {
        dst += count;
        src += count;
        while (count > 0U)
        {
            --dst;
            --src;
            *dst = *src;
            --count;
        }
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

void __aeabi_memclr4(void *destination, size_t count)
{
    (void)memset(destination, 0, count);
}
