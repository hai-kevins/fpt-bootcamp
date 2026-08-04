#include <stddef.h>
#include <stdint.h>

void *memcpy(void *destination, const void *source, size_t count)
{
    uint8_t *dst = (uint8_t *) destination;
    const uint8_t *src = (const uint8_t *) source;

    while (count > 0U)
    {
        *dst++ = *src++;
        count--;
    }

    return destination;
}

void *memset(void *destination, int value, size_t count)
{
    uint8_t *dst = (uint8_t *) destination;

    while (count > 0U)
    {
        *dst++ = (uint8_t) value;
        count--;
    }

    return destination;
}

int memcmp(const void *left, const void *right, size_t count)
{
    const uint8_t *a = (const uint8_t *) left;
    const uint8_t *b = (const uint8_t *) right;

    while (count > 0U)
    {
        if (*a != *b)
        {
            return (*a < *b) ? -1 : 1;
        }
        a++;
        b++;
        count--;
    }

    return 0;
}

void __aeabi_memcpy(void *destination, const void *source, size_t count)
{
    (void) memcpy(destination, source, count);
}

void __aeabi_memcpy4(void *destination, const void *source, size_t count)
{
    (void) memcpy(destination, source, count);
}

void __aeabi_memcpy8(void *destination, const void *source, size_t count)
{
    (void) memcpy(destination, source, count);
}

void __aeabi_memset(void *destination, size_t count, int value)
{
    (void) memset(destination, value, count);
}

void __aeabi_memset4(void *destination, size_t count, int value)
{
    (void) memset(destination, value, count);
}

void __aeabi_memclr(void *destination, size_t count)
{
    (void) memset(destination, 0, count);
}

void __aeabi_memclr4(void *destination, size_t count)
{
    (void) memset(destination, 0, count);
}

uint32_t __aeabi_uidiv(uint32_t numerator, uint32_t denominator)
{
    uint32_t quotient = 0UL;
    uint32_t remainder = 0UL;
    int32_t bit;

    if (denominator == 0UL)
    {
        return 0UL;
    }

    for (bit = 31; bit >= 0; bit--)
    {
        remainder = (remainder << 1U) | ((numerator >> (uint32_t) bit) & 1UL);

        if (remainder >= denominator)
        {
            remainder -= denominator;
            quotient |= 1UL << (uint32_t) bit;
        }
    }

    return quotient;
}

uint64_t __aeabi_uidivmod(uint32_t numerator, uint32_t denominator)
{
    const uint32_t quotient = __aeabi_uidiv(numerator, denominator);
    const uint32_t remainder = (denominator == 0UL) ? 0UL : numerator - (quotient * denominator);
    return ((uint64_t) remainder << 32U) | quotient;
}
