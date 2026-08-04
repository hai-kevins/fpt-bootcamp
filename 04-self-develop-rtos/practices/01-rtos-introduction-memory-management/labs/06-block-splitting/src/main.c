#include "heap.h"
#include <stdint.h>
#include <stdio.h>

int main(void)
{
    uint8_t memory[512];
    void *a;
    void *b;
    void *c;
    printf("Lab 06 - Block splitting\n");
    if (!heap_init(memory, sizeof(memory)))
    {
        return 1;
    }
    a = heap_alloc(32U);
    b = heap_alloc(48U);
    c = heap_alloc(64U);
    printf("A=%p B=%p C=%p blocks=%zu validator=%s\n", a, b, c, heap_block_count(), heap_validate() ? "PASS" : "FAIL");
    (void) heap_free(b);
    printf("After free B, blocks=%zu (no coalescing yet)\n", heap_block_count());
    return 0;
}
