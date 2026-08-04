#include "heap.h"
#include <stdint.h>
#include <stdio.h>

int main(void)
{
    uint8_t memory[512];
    uint8_t foreign;
    void *a;
    heap_status_t first;
    heap_status_t second;
    heap_status_t bad;
    printf("Lab 08 - Invalid free detection\n");
    if (heap_init(memory, sizeof(memory)) != HEAP_STATUS_OK)
    {
        return 1;
    }
    a = heap_alloc(32U);
    bad = heap_free(&foreign);
    first = heap_free(a);
    second = heap_free(a);
    printf("Foreign pointer status=%d\n", (int) bad);
    printf("First free status=%d\n", (int) first);
    printf("Double free status=%d\n", (int) second);
    printf("Validator=%s\n", heap_validate() ? "PASS" : "FAIL");
    return 0;
}
