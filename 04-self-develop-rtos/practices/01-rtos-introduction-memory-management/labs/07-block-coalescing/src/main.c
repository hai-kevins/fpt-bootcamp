#include "heap.h"
#include <stdint.h>
#include <stdio.h>

int main(void)
{
    uint8_t memory[512];
    void *a;
    void *b;
    void *c;
    printf("Lab 07 - Block coalescing\n");
    if (!heap_init(memory, sizeof(memory)))
    {
        return 1;
    }
    a = heap_alloc(32U);
    b = heap_alloc(48U);
    c = heap_alloc(64U);
    printf("Before free: blocks=%zu\n", heap_block_count());
    (void) heap_free(b);
    (void) heap_free(a);
    (void) heap_free(c);
    printf("After free all: blocks=%zu validator=%s\n", heap_block_count(), heap_validate() ? "PASS" : "FAIL");
    return 0;
}
