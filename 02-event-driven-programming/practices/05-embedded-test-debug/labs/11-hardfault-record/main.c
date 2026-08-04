#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef struct
{
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r12;
    uint32_t lr;
    uint32_t pc;
    uint32_t xpsr;
    uint32_t cfsr;
    uint32_t hfsr;
    uint32_t mmfar;
    uint32_t bfar;
} hardfault_record_t;

int main(void)
{
    const hardfault_record_t record =
    {
        .lr = 0xFFFFFFF9UL,
        .pc = 0x08001234UL,
        .xpsr = 0x21000000UL,
        .cfsr = 0x00008200UL,
        .hfsr = 0x40000000UL,
        .mmfar = 0x20010000UL,
        .bfar = 0x40000001UL
    };

    const bool pass = (record.pc != 0U) && (record.lr != 0U) && (record.xpsr != 0U) && (record.cfsr != 0U);

    (void) printf("pc=0x%08lX lr=0x%08lX xpsr=0x%08lX ""cfsr=0x%08lX %s\n", (unsigned long) record.pc, (unsigned long) record.lr,
        (unsigned long) record.xpsr, (unsigned long) record.cfsr, pass ? "PASS" : "FAIL");

    return pass ? 0 : 1;
}
