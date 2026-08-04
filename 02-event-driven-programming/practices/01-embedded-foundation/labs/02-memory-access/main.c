#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

static uint32_t memory_read32(uintptr_t address)
{
    const volatile uint32_t *pointer = (const volatile uint32_t *) address;

    return *pointer;
}

static void memory_write32(uintptr_t address, uint32_t value)
{
    volatile uint32_t *pointer = (volatile uint32_t *) address;
    *pointer = value;
}

int main(void)
{
    volatile uint32_t test_value = UINT32_C(0x12345678);
    const uintptr_t address = (uintptr_t)(const void *)&test_value;

    printf("test_value address = 0x%" PRIXPTR"\n", address);
    printf("initial value = 0x%08" PRIX32"\n", memory_read32(address));

    memory_write32(address, UINT32_C(0xABCDEF01));

    printf("after write = 0x%08" PRIX32"\n", memory_read32(address));

    return 0;
}
