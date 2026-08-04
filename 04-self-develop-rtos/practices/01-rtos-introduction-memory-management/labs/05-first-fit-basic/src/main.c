#include "heap.h"
#include <stdint.h>
#include <stdio.h>

int main(void)
{
    uint8_t memory[256];
    void *a;
    printf("Lab 05 - First-fit basic\n");
    if (!heap_init(memory, sizeof(memory)))
    {
        return 1;
    }
    printf("Capacity: %zu bytes\n", heap_capacity());
    a = heap_alloc(32U);
    printf("Allocate 32: %s\n", a != NULL ? "PASS" : "FAIL");
    printf("Second allocation without free: %s\n", heap_alloc(16U) == NULL ? "REJECTED" : "UNEXPECTED");
    printf("Free: %s\n", heap_free(a) ? "PASS" : "FAIL");
    return 0;
}
