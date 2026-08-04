#include "test.h"
#include "heap.h"
#include <stdint.h>

int test_double_free(void)
{
    uint8_t memory[512];
    void *pointer;
    TEST_ASSERT(heap_init(memory, sizeof(memory)) == HEAP_STATUS_OK);
    pointer = heap_alloc(32U);
    TEST_ASSERT(pointer != (void *)0);
    TEST_ASSERT(heap_free(pointer) == HEAP_STATUS_OK);
    TEST_ASSERT(heap_free(pointer) == HEAP_STATUS_DOUBLE_FREE);
    TEST_ASSERT(heap_validate());
    return 0;
}
